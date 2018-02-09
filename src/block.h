#ifndef BLOCK_H
#define BLOCK_H

#include <memory>

// Block Types
#define BLOCK_AIR 0
#define BLOCK_EARTH 1
#define BLOCK_BOX 2

// Texture atlas
#define TEX_HEIGHT 3
#define TEX_WIDTH 2

// Texture types
#define TEX_ONE 1
#define TEX_TWO 2

// Texture misc
#define TEX_ERROR -1

namespace cuboc {
  
  struct BaseParams {};

  struct Block {
    Block() : type_(BLOCK_AIR) {}
    Block(short type) : type_(type) {}

    short getType() { return type_; }
    
    short getTexRow() {
      switch(type_) {
        case BLOCK_EARTH:
          return 0;
        case BLOCK_BOX:
          return 2;
        
      }
      return TEX_ERROR; // Means N/A type
    }

    char getTexType() {
      switch(type_) {
        case BLOCK_EARTH:
          return TEX_ONE;
        case BLOCK_BOX:
          return TEX_TWO;
      }
      return TEX_ERROR;
    }

    short type_;
    std::shared_ptr<BaseParams> params;
  };

}

#endif
