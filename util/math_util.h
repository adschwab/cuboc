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

#endif
