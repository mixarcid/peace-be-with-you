#include "TerrainGenerator.hpp"
#include "Matrix.hpp"
#include "Noise.hpp"

NAMESPACE {

  TerrainGenerator::TerrainGenerator() {}

  f32 TerrainGenerator::heightAtPoint(Vec2f pos) {
    return 0;
    /*Noise::fractal
      ([this](Vec2f p, i32 i)->f32 {
	return 1.0f-abs(perlin.getValue(p));
	}, pos/100, 8);*/
    /*f32 c = perlin.getValue(pos/20-Vec2f(354324,6798043));
    f32 s = sqrt(1-c);
    f32 vals[4] = {c,-s,s,c};
    Mat2f rot(vals);
    f32 ret = perlin.getValue(pos/20);
    return 20*(ret-0.5);*/
  }
  Vec2f TerrainGenerator::texCoordAtPoint(Vec2f pos) {
    return Vec2f(0,0);
  }
  
}
