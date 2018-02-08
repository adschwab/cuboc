#ifndef BLOCK_H
#define BLOCK_H

#include <memory>

// Block Types
#define BLOCK_AIR 0
#define BLOCK_EARTH 1


#define TEX_HEIGHT 3
#define TEX_WIDTH 2

// Texture types
#define TEX_ONE 1
#define TEX_TWO 2

namespace cuboc {
  
  struct BaseParams {};

  struct BaseBlock {
    BaseBlock(short type) : type_(type) {}

    short getType() { return type_; }
    virtual short getTexRow() = 0;
    virtual char getTexType() = 0;

    short type_;
    std::unique_ptr<BaseParams> params;
  };

  struct GrassBlock : public BaseBlock {
    GrassBlock() :
        BaseBlock(BLOCK_EARTH) {}
    virtual short getTexRow() {
      return 0;
    }
    virtual char getTexType() {
      return TEX_ONE;
    }
  };

  struct AirBlock : public BaseBlock {
    AirBlock() :
        BaseBlock(BLOCK_AIR) {}
  };
}

#endif
