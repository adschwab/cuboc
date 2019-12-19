#include "util/math_util.h"

#include <cstdio>
#include <cmath>
#include <vector>
#include <array>
#include <limits>
#include <cfloat>

#include <glm/glm.hpp>

#include "util/macro.h"

namespace cuboc {

void calcDiffs(
    glm::vec3 dir,
    float dist,
    glm::vec3 &diffs) {

  diffs[0] = dir[0] * dist;
  diffs[1] = dir[1] * dist;
  diffs[2] = dir[2] * dist;
}  
  
void walkLine(
    const glm::vec3 pos,
    const glm::vec3 dir, // This is assumed to be normalized
    float dist, float size,
    std::function<bool(std::array<int, 3>)> func) {
  dist = dist / size;

  float gx0 = pos[0] / size;
  float gy0 = pos[1] / size;
  float gz0 = pos[2] / size;
  
  float dx = dir[0];
  float dy = dir[1];
  float dz = dir[2];

  float gx1 = gx0 + dx * dist;
  float gy1 = gy0 + dy * dist;
  float gz1 = gz0 + dz * dist;

  int gx0idx = (int)gx0;
  int gy0idx = (int)gy0;
  int gz0idx = (int)gz0;

  int gx1idx = (int)gx1;
  int gy1idx = (int)gy1;
  int gz1idx = (int)gz1;

  int sx = gx1idx > gx0idx ? 1 : gx1idx < gx0idx ? -1 : 0;
  int sy = gy1idx > gy0idx ? 1 : gy1idx < gy0idx ? -1 : 0;
  int sz = gz1idx > gz0idx ? 1 : gz1idx < gz0idx ? -1 : 0;

  int gx = gx0idx;
  int gy = gy0idx;
  int gz = gz0idx;

  //Planes for each axis that we will next cross
  int gxp = gx0idx + (gx1idx > gx0idx ? 1 : 0);
  int gyp = gy0idx + (gy1idx > gy0idx ? 1 : 0);
  int gzp = gz0idx + (gz1idx > gz0idx ? 1 : 0);

  //Only used for multiplying up the error margins
  float vx = gx1 == gx0 ? 1.0f : gx1 - gx0;
  float vy = gy1 == gy0 ? 1.0f : gy1 - gy0;
  float vz = gz1 == gz0 ? 1.0f : gz1 - gz0;

  //Error is normalized to vx * vy * vz so we only have to multiply up
  float vxvy = vx * vy;
  float vxvz = vx * vz;
  float vyvz = vy * vz;

  //Error from the next plane accumulators, scaled up by vx*vy*vz
  float errx = (float)(gxp - gx0) * vyvz;
  float erry = (float)(gyp - gy0) * vxvz;
  float errz = (float)(gzp - gz0) * vxvy;

  float derrx = (float)sx * vyvz;
  float derry = (float)sy * vxvz;
  float derrz = (float)sz * vxvy;

  while (true) {
    if (!func({gx, gy, gz})) break;
    if (gx == gx1idx && gy == gy1idx && gz == gz1idx) break;

    //Which plane do we cross first?
    float xr = std::abs(errx);
    float yr = std::abs(erry);
    float zr = std::abs(errz);

    if (sx != 0 && (sy == 0 || xr < yr) && (sz == 0 || xr < zr)) {
      gx += sx;
      errx += derrx;
    }
    else if (sy != 0 && (sz == 0 || yr < zr)) {
      gy += sy;
      erry += derry;
    }
    else if (sz != 0) {
      gz += sz;
      errz += derrz;
    }

  }
}


float distPlane(
    bool is_positive,
    char dim_ind,
    float plane,
    glm::vec3 pos,
    glm::vec3 dir) {
  
  float diffs[3];
  float sign_mult = is_positive ? 1.0f : -1.0f;
  if (dir[dim_ind] == 0.0f)
    return std::numeric_limits<float>::infinity();
  float p = (plane - pos[dim_ind])/dir[dim_ind];
  sign_mult *= (p > 0.0f ? 1.0f : -1.0f);
  diffs[dim_ind] = p * dir[dim_ind] + pos[dim_ind];
  for (int i = 0; i < 3; i ++) {
    if (i != dim_ind) {
      diffs[i] = p * dir[i] + pos[i];
    }
  }

  return sign_mult * std::sqrt(
      SQUARE(diffs[0] - pos[0]) + 
      SQUARE(diffs[1] - pos[1]) + 
      SQUARE(diffs[2] - pos[2]));
}

glm::vec3 calcDir(float xy, float yz) {
  float x = std::sin(xy) * std::cos(yz);
  float y = std::cos(xy) * std::cos(yz);
  float z = std::sin(yz);

  return glm::vec3(x, y, z);
}

glm::vec3 calcDirXY(float xoff, float yoff) {
  if (yoff == 0.0f) return calcDir(PI/2.0f, 0.0f);
  float xy = std::atan(xoff / yoff);
  return calcDir(xy, 0.0f);
}

glm::vec3 crossProduct(glm::vec3 p1, glm::vec3 p2) {
  glm::vec3 cp;
  cp[0] = p1[1] * p2[2] - p1[2] * p2[1];
  cp[1] = p1[2] * p2[0] - p1[0] * p2[2];
  cp[2] = p1[0] * p2[1] - p1[1] * p2[0];
  return cp;
}

float dotProduct(glm::vec3 p1, glm::vec3 p2) {
  return p1[0] * p2[0] + p1[1] * p2[1] + p1[2] * p2[2];
}

glm::vec3 solve(glm::mat3 m, glm::vec3 b) {
  glm::mat3 l;
  glm::mat3 u;

  std::array<int, 3> o = {0, 1, 2};
  for (int i = 0; i < 3; i++) {
    float max = std::abs(m[o[i]][i]);
    int max_ind = i;
    for (int j = i + 1; j < 3; j ++) {
      if (std::abs(m[o[j]][i]) > max) {
        max = std::abs(m[o[j]][i]);
        max_ind = j;
      }
    }
    int tmp = o[i];
    o[i] = o[max_ind];
    o[max_ind] = tmp;
    
    for (int k = i; k < 3; k++) {
      int sum = 0.0f;
      for (int j = 0; j < i; j++)
        sum += (l[i][j] * u[j][k]);
      u[i][k] = m[o[i]][k] - sum;
    }
    for (int k = i; k < 3; k++) {
      if (i == k)
        l[i][i] = 1.0f; // Diagonal as 1
      else {
        int sum = 0.0f;
        for (int j = 0; j < i; j++)
          sum += (l[k][j] * u[j][i]);
        l[k][i] = (m[o[k]][i] - sum) / u[i][i];
      }
    }
  }

  
  std::printf("l:\n");
  std::printf("%.2f,%.2f,%.2f\n", l[0][0], l[0][1], l[0][2]);
  std::printf("%.2f,%.2f,%.2f\n", l[1][0], l[1][1], l[1][2]);
  std::printf("%.2f,%.2f,%.2f\n", l[2][0], l[2][1], l[2][2]);
  std::printf("u:\n");
  std::printf("%.2f,%.2f,%.2f\n", u[0][0], u[0][1], u[0][2]);
  std::printf("%.2f,%.2f,%.2f\n", u[1][0], u[1][1], u[1][2]);
  std::printf("%.2f,%.2f,%.2f\n", u[2][0], u[2][1], u[2][2]);
  

  glm::vec3 y;
  for(int i = 0; i < 3; i++){
    float s = 0.0f;
    for(int j = 0; j < i; j++){
      s = s + l[i][j] * y[j];
    }
    if (l[i][i] == 0.0f)
      l[i][i] = FLT_MIN;
    y[i] = (b[o[i]] - s) / l[i][i];
  }
  
  glm::vec3 x; 
  for(int i = 2; i >= 0; i--){
    float s = 0.0f;
    for(int j = 2; j > i; j--){
      s = s + u[i][j] * x[j];
    }
    if (u[i][i] == 0.0f)
      u[i][i] = FLT_MIN;
    x[i] = (y[i] - s) / u[i][i];
  }
  
  return x;
}

float norm(glm::vec3 pt1, glm::vec3 pt2) {

  return std::sqrt(SQUARE(pt2[0] - pt1[0]) + SQUARE(pt2[1] - pt1[1]) + SQUARE(pt2[2] - pt1[2]));
}

inline bool findBounds(int p1, int p2,
    int c, int size,
    int &b1, int &b2) {
  if (p1 > p2)
    return false;

  if (c >= p1 && c + size < p1)
    b1 = c;
  else if (c + size >= p1)
    return false;
  else
    b1 = p1;

  if (p2 >= c + size - 1)
    b2 = c + size - 1;
  else
    b2 = p2;
  return true; 
}

bool findBounds(int x1, int x2,
    int y1, int y2,
    int z1, int z2,
    int cx, int cy, int cz, 
    int size,
    int &bx1, int &bx2,
    int &by1, int &by2, 
    int &bz1, int &bz2) {
  std::printf("(%d, %d, %d), (%d, %d, %d)", x1, y1, z1, x2, y2, z2);
  if (!findBounds(x1, x2, cx, size, bx1, bx2))
    return false;
  if (!findBounds(y1, y2, cy, size, by1, by2))
    return false;
  return findBounds(z1, z2, cz, size, bz1, bz2);
}

} // namespace
