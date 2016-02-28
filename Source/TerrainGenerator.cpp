#include "TerrainGenerator.hpp"
#include "Matrix.hpp"
#include "Noise.hpp"

NAMESPACE {

  const static f32 OFFSET = 1000.0f;

  TerrainGenerator::TerrainGenerator() {}

  f32 TerrainGenerator::heightAtPoint(Vec2f pos) {
    f32 ret = Noise::fractal
      ([this](Vec2f p, i32 i)->f32 {
	return /*1.0f-abs*/(perlin.getValue(p+Vec2f(OFFSET, OFFSET)));
	}, pos/200, 4);
    //perlin.getValue(pos/20);
    return 50*(ret-0.5);
  }
  
  Vec2f TerrainGenerator::texCoordAtPoint(Vec2f pos) {
    return Vec2f(0,0);
  }
  
}
