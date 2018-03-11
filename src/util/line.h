#ifndef UTIL_LINE_H
#define UTIL_LINE_H

#include <glm/glm.hpp>

#include "util/math_util.h"

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

    float distLine(Line other) {
      
      glm::vec3 l = crossProduct(dir, other.dir);
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
      /*
      std::printf("m:\n");
      std::printf("%.2f,%.2f,%.2f\n", m[0][0], m[0][1], m[0][2]);
      std::printf("%.2f,%.2f,%.2f\n", m[1][0], m[1][1], m[1][2]);
      std::printf("%.2f,%.2f,%.2f\n", m[2][0], m[2][1], m[2][2]);
      std::printf("b: %.2f,%.2f,%.2f\n", b[0], b[1], b[2]);
      std::printf("params: %.2f,%.2f,%.2f\n", params[0], params[1], params[2]);
      */
      glm::vec3 pt1 = getPoint(params[0]);
      glm::vec3 pt2 = other.getPoint(params[1]);
      /*
      std::printf("pt1: %.2f,%.2f,%.2f\n", pt1[0], pt1[1], pt1[2]);
      std::printf("pt2: %.2f,%.2f,%.2f\n", pt2[0], pt2[1], pt2[2]);
      */

      return norm(pt1, pt2);
    }
  };

}


#endif
