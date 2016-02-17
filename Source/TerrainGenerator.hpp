#pragma once
#include "Perlin.hpp"

NAMESPACE {

  struct TerrainGenerator {

    Perlin perlin;
    
    TerrainGenerator();
    f32 heightAtPoint(Vec2f pos);
    Vec2f texCoordAtPoint(Vec2f pos);

  };

}
