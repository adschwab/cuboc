#ifndef UTIL_LINE_H
#define UTIL_LINE_H

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/math_util.h"

#define MIN_THRESHOLD 0.00001f

namespace cuboc {

struct Line {

  glm::vec3 pos;
  glm::vec3 dir;

  glm::vec3 getPoint(float param) {
    return glm::vec3(
        pos[0] + param * dir[0],
        pos[1] + param * dir[1],
        pos[2] + param * dir[2]);
  }

  float getAngle(Line other) {
    return std::acos(dotProduct(dir, other.dir));
  }

  float distLine(Line other) {
    float acos = dotProduct(dir, other.dir);   
    if (1.0f - std::abs(acos) <= MIN_THRESHOLD) {
      // we can pick any point
      glm::vec3 p1 = pos;
      glm::vec3 p2 = other.pos;
      glm::vec3 diff = glm::normalize(p2 - p1);
     

      acos = dotProduct(diff, dir);
      if (1.0f - std::abs(acos) <= MIN_THRESHOLD) {
        return 0.0f;
      }

      glm::vec3 l = crossProduct(dir, 
          crossProduct(diff, dir));
      glm::vec3 newl = crossProduct(l, other.dir);
      //std::printf("DIFF: %.2f,%.2f,%.2f\n", diff[0], diff[1], diff[2]);
      //std::printf("DIR:  %.2f,%.2f,%.2f\n", dir[0], dir[1], dir[2]);
      //std::printf("PROD: %.2f,%.2f,%.2f\n", l[0], l[1], l[2]);
      
      // Find closest point to new line
      glm::vec3 b;
      for (int i = 0; i < 3; i ++)
        b[i] = other.pos[i] - p1[i];

      glm::mat3 m;
      for (int i = 0; i < 3; i ++) {
        m[i][0] = l[i];
        m[i][1] = -other.dir[i];
        m[i][2] = newl[i];
      }

      glm::vec3 params = solve(m, b);
       
      p2 = other.getPoint(params[1]);
      return norm(p1, p2);
    }

    glm::vec3 l = crossProduct(dir, other.dir);
    std::printf("cp: %.2f,%.2f,%.2f\n", l[0], l[1], l[2]);

    // P1 + ul = P2
    glm::vec3 b;
    for (int i = 0; i < 3; i ++)
      b[i] = other.pos[i] - pos[i];

    glm::mat3 m;
    for (int i = 0; i < 3; i ++) {
      m[i][0] = dir[i];
      m[i][1] = -other.dir[i];
      m[i][2] = l[i];
    }

    glm::vec3 params = solve(m, b);
    
    std::printf("m:\n");
    std::printf("%.2f,%.2f,%.2f\n", m[0][0], m[0][1], m[0][2]);
    std::printf("%.2f,%.2f,%.2f\n", m[1][0], m[1][1], m[1][2]);
    std::printf("%.2f,%.2f,%.2f\n", m[2][0], m[2][1], m[2][2]);
    std::printf("b: %.2f,%.2f,%.2f\n", b[0], b[1], b[2]);
    std::printf("params: %.2f,%.2f,%.2f\n", params[0], params[1], params[2]);
    
    glm::vec3 pt1 = getPoint(params[0]);
    glm::vec3 pt2 = other.getPoint(params[1]);
    
    std::printf("pt1: %.2f,%.2f,%.2f\n", pt1[0], pt1[1], pt1[2]);
    std::printf("pt2: %.2f,%.2f,%.2f\n", pt2[0], pt2[1], pt2[2]);
    

    return norm(pt1, pt2);
  }
};

}


#endif
