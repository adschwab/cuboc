#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include <glm/glm.hpp>


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
    const glm::vec3 &pos,
    const glm::vec3 &dir, // This is assumed to be normalized
    float dist,
    bool visit(int, int, int));

/*
 * xy:   Angle from global forward on horizontal plane
 * yz:   Angle from global xy on xyz plane
 *
 */
glm::vec3 calcDir(float xy, float yz);

bool findBounds(int x1, int x2,
    int y1, int y2,
    int z1, int z2,
    int cx, int cy, int cz, int size,
    int &bx1, int &bx2,
    int &by1, int &by2,
    int &bz1, int &bz2);

#endif
