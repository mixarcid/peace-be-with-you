#pragma once

#include "StaticMesh.hpp"
#include "Vector.hpp"

NAMESPACE {

  struct Terrain {

    Array<StaticMesh> chunks;
    
    void generate(Vec2<u32> size);
    
  };

}
