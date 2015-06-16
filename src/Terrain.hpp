#pragma once

#include "StaticMesh.hpp"
#include "VectorMath.hpp"

NAMESPACE {

  struct Terrain {

    Array<StaticMesh> chunks;
    
    void generate(Vec2<unsigned int> size);
    
  };

}
