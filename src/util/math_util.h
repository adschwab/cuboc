#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include <vector>
#include <array>

#include <glm/glm.hpp>

#include "util/log.h"

#define PI 3.141593f

namespace cuboc {

void calcDiffs(
    glm::vec3 dir,
    float dist,
    glm::vec3 &diffs); 
  
/*
 * pos:    the position in xyz of object
 * dir:    the direction, assumed to be normalized
 * dist:   how far we will look
 * visit:  function called with each cube coordinates
 *         starting from pos. Returns whether to continue:
 *           true:  continue walking
 *           false: stop
 */
void walkLine(
    const glm::vec3 pos,
    const glm::vec3 dir, // This is assumed to be normalized
    float dist, float size,
    std::vector<std::array<int, 3> > &offsets);


/*
 * dim_ind: which dim is the plane?
 * plane:   the plane's position in dim_ind
 * pos:     the position in xyz of object
 * dir:     the direction, assumed to be normalized
 */
float distPlane(
    bool is_positive,
    char dim_ind,
    float plane,
    glm::vec3 pos,
    glm::vec3 dir);

glm::vec3 crossProduct(glm::vec3 p1, glm::vec3 p2);

float dotProduct(glm::vec3 p1, glm::vec3 p2);

glm::vec3 solve(glm::mat3 m, glm::vec3 b);


/*
 * xy:   Angle from global forward on horizontal plane
 * yz:   Angle from global xy on xyz plane
 *
 * Returns: a normalized 3d vector direction
 */
glm::vec3 calcDir(float xy, float yz);

/*
 * xoff:   x offset
 * yoff:   y offset
 *
 * Returns: a normalized 3d vector direction
 */
glm::vec3 calcDirXY(float xoff, float yoff);

float norm(glm::vec3 pt1, glm::vec3 pt2);

bool findBounds(int x1, int x2,
    int y1, int y2,
    int z1, int z2,
    int cx, int cy, int cz, int size,
    int &bx1, int &bx2,
    int &by1, int &by2,
    int &bz1, int &bz2);

} // namespace

#endif
