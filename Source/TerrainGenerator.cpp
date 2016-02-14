#include "TerrainGenerator.hpp"

NAMESPACE {

  BasicMeshData TerrainGenerator::dataAtPos(Vec2f pos) {
    return BasicMeshData(Vec3f(pos.x(),pos.y(),0),
			 Vec3f(0,0,1),
			 Vec2f(0,0));
  }
  
}
