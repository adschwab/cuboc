#include "util/math_util.h"

#include <cstdio>
#include <cmath>

#include <glm/glm.hpp>

#define PI 3.141593f
void walkLine(
    const glm::vec3 &pos,
    const glm::vec3 &dir, // This is assumed to be normalized
    float dist,
    bool visit(int, int, int)) {

  float gx0 = pos[0];
  float gy0 = pos[1];
  float gz0 = pos[2];

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
    if (!visit(gx, gy, gz)) break;

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

glm::vec3 calcDir(float xy, float yz) {
  float x = std::sin(xy) * std::cos(yz);
  float y = std::cos(xy) * std::cos(yz);
  float z = std::sin(yz);

  return glm::vec3(x, y, z);
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


