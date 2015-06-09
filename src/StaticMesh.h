#pragma once

#include "Standard.h"
#include "Renderable.h"
#include "VectorMath.h"
#include "Containers.h"

NAMESPACE {

  struct StaticMeshData {

    Vec3f pos;
    Vec3f norm;

    StaticMeshData(Vec3f position, Vec3f normal) {
      pos = position;
      norm = normal;
    }

  };

  struct StaticMesh : RenderableEBO<StaticMeshData> {

    StaticMesh() : RenderableEBO<StaticMeshData>() {}

    void init(Array<StaticMeshData> mesh_data,
	      Array<GLuint> mesh_elems) {
      Renderable<StaticMeshData>::data = mesh_data;
      RenderableEBO<StaticMeshData>::elements = mesh_elems;
      log::message("Element size: %u", mesh_elems.size());
      RenderableEBO<StaticMeshData>::init({Shader::POSITION,
	    Shader::NORMAL}, false);
    }
    
  };
  
}
