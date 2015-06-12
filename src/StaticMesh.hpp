#pragma once

#include "Standard.hpp"
#include "Renderable.hpp"
#include "VectorMath.hpp"
#include "Containers.hpp"
#include "String.hpp"
#include "Texture.hpp"

NAMESPACE {

  struct StaticMeshData {

    Vec3f pos;
    Vec3f norm;
    Vec2f tex_coord;

    StaticMeshData(Vec3f position, Vec3f normal,
		   Vec2f texture_coord) {
      pos = position;
      norm = normal;
      tex_coord = texture_coord;
    }

  };

  struct StaticMesh : RenderableEBO<StaticMeshData> {

    Texture tex;
    
    StaticMesh(Array<StaticMeshData> mesh_data,
	       Array<GLuint> mesh_elems, Texture* texture);
    virtual void render();
    
  };
  
}
