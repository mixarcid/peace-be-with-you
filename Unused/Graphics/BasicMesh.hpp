#pragma once

#include "Containers.hpp"
#include "Vector.hpp"
#include "Shader.hpp"

NAMESPACE {

  struct BasicMeshData {

    Vec3f pos;
    Vec3f norm;
    Vec2f tex_coord;

    BasicMeshData(Vec3f position,
		  Vec3f normal,
		  Vec2f texture_coord)
      : pos(position),
	norm(normal),
	tex_coord(texture_coord) {}

  };

}
