#ifndef BLOCK_H
#define BLOCK_H

#include <memory>

// Block Types
#define BLOCK_AIR 0
#define BLOCK_EARTH 1

// Texture types
#define TEX_ONE 1

namespace cuboc {

  struct GrassBlock : BaseBlock {
    GrassBlock() :
        type(BLOCK_EARTH) {}
    virtual short getTexRow() {
      return 0;
    }
    virtual char getTexType() {
      return TEX_ONE;
    }
  }

  struct AirBlock : BaseBlock {
    AirBlock() :
        type(BLOCK_AIR) {}
  } 

  struct BaseParams {};

  struct BaseBlock {
    short getType() { return type; }
    virtual short getTexRow() = 0;
    virtual char getTexType() = 0;

    short type;
    std::unique_ptr<BaseParams> params;
  };
}

#endif
