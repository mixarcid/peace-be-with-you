#include "TerrainGenerator.hpp"
#include "Noise.hpp"

NAMESPACE {

  TerrainGenerator::TerrainGenerator() {}

  f32 TerrainGenerator::heightAtPoint(Vec2f pos) {
    f32 ret = Noise::fractal
      ([this](Vec2f p, i32 i)->f32 {
	return 1.0f-abs(perlin.getValue(p));
      }, pos/100, 8);
    return 20*(ret-0.5);
  }
  Vec2f TerrainGenerator::texCoordAtPoint(Vec2f pos) {
    return Vec2f(0,0);
  }
  
}
