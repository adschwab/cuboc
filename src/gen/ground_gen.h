#ifndef GROUND_GEN_H
#define GROUND_GEN_H

#include <memory>

#include "objects/ground.h"

std::shared_ptr<std::vector<GLfloat>> gen_ground_data(
    int xsz,
    int ysz);

#endif // inclusion guard
