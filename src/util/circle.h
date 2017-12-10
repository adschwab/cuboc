#ifndef CIRCLE_H
#define CIRCLE_H

#include <vector>
#include <math.h>
#include <unordered_set>
#include <queue>

#include "util/hashkey.h"


bool nextTo(XYCoord coord1, XYCoord coord2, int block_size) {
  float xstep[4] = {(float)block_size, 0.0f, -(float)block_size, 0.0f};
  float ystep[4] = {0.0f, (float)block_size, 0.0f, -(float)block_size};
  for (int i = 0; i < 4; i ++) {
    if (coord2.x() == coord1.x() + xstep[i] &&
        coord2.y() == coord1.y() + ystep[i]) {
      return true;
    }
  }
  return false;
}

XYCoord getXYCoord(float x, float y, int block_size) {

  float x_ = (float) ((int)(x / (float) block_size 
        + (x > 0.0f ? 1.0f : -1.0f) * 0.5f) * block_size);
  float y_ = (float) ((int)(y / (float) block_size 
        + (y > 0.0f ? 1.0f : -1.0f) * 0.5f) * block_size);

  return XYCoord(x_, y_);
}

void genCircleHelper(
    XYCoord origin,
    float param_start,
    float param_end,
    int block_size,
    float radius,
    std::unordered_set<XYCoord> &boundary) {
  
  float xs = radius * cos(param_start);
  float ys = radius * sin(param_start);

  XYCoord start = getXYCoord(
      origin.x() + xs,
      origin.y() + ys,
      block_size);

  float xe = radius * cos(param_end);
  float ye = radius * sin(param_end);
  XYCoord end = getXYCoord(
      origin.x() + xe,
      origin.y() + ye,
      block_size);

  boundary.insert(start);
  boundary.insert(end);  

  if (start != end && !nextTo(start, end, block_size)) {
    genCircleHelper(
        origin,
        param_start,
        (param_start + param_end) / 2,
        block_size,
        radius,
        boundary);
    genCircleHelper(
        origin,
        (param_start + param_end) / 2,
        param_end,
        block_size,
        radius,
        boundary);
  }
}

void flood(
    XYCoord origin,
    int block_size,
    std::unordered_set<XYCoord> &boundary,
    std::vector<XYCoord> &out_coords) {
  
  float xstep[4] = {(float)block_size, 0.0f, -(float)block_size, 0.0f};
  float ystep[4] = {0.0f, (float)block_size, 0.0f, -(float)block_size};
  std::queue<XYCoord> queue;
  queue.push(origin);
  while (queue.size() > 0) {
    XYCoord coord = queue.front();
    queue.pop();
    if (boundary.find(coord) == boundary.end()) {
      out_coords.push_back(coord);
      boundary.insert(coord);
      for (int i = 0; i < 4; ++i) {
        XYCoord newcoord(coord.x() + xstep[i], coord.y() + ystep[i]);
        queue.push(newcoord);
      }
    }
    
  }

}

void genCircle(
    XYCoord origin,
    float radius,
    int block_size,
    std::vector<XYCoord> &out_coords) {
  
  XYCoord norm_origin =
      getXYCoord(origin.x(), origin.y(), block_size);

  std::unordered_set<XYCoord> boundary;

  float param_start = 0.0f;
  float param_end = M_PI;
  genCircleHelper(
      norm_origin,
      param_start,
      param_end,
      block_size,
      radius,
      boundary);  
  
  param_start = M_PI;
  param_end = 2.0f * M_PI;
  genCircleHelper(
      norm_origin,
      param_start,
      param_end,
      block_size,
      radius,
      boundary);  

  for (auto it = boundary.begin(); it != boundary.end(); ++ it) {
    //std::printf("(%.2f, %.2f)\n", it->x(), it->y());
    out_coords.push_back(*it);
  }

  flood(
      norm_origin,
      block_size,
      boundary,
      out_coords); 
}



#endif
