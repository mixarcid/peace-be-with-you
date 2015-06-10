#pragma once

#include "Standard.hpp"
#include "Renderable.hpp"
#include "VectorMath.hpp"
#include "Containers.hpp"

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

    /*StaticMesh() : RenderableEBO<StaticMeshData>() {}

    void init() {
      Renderable<StaticMeshData>::data = mesh_data;
      RenderableEBO<StaticMeshData>::elements = mesh_elems;
      //log::message("Element size: %u", mesh_elems.size());
      RenderableEBO<StaticMeshData>::init({Shader::POSITION,
	    Shader::NORMAL}, false);
    }
    
    };*/
    
    StaticMesh(Array<StaticMeshData> mesh_data,
	       Array<GLuint> mesh_elems) {

      Renderable<StaticMeshData>::data = mesh_data;
      RenderableEBO<StaticMeshData>::elements = mesh_elems;
      RenderableEBO<StaticMeshData>::init({Shader::POSITION,
	    Shader::NORMAL}, false);
      
    }
    
  };
  
}
