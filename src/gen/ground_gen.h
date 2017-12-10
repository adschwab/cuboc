#ifndef GROUND_GEN_H
#define GROUND_GEN_H

#include <memory>

#include "world.h"

#include "objects/ground.h"

void gen_ground_data(
    std::unordered_map<XYCoord, Block> &blocks,
    std::shared_ptr<std::vector<GLfloat> > out_heights,
    std::shared_ptr<std::vector<int> > out_tex_ids);

#endif // inclusion guard
