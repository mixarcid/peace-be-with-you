#include "StaticMesh.hpp"

NAMESPACE {

  StaticMesh::StaticMesh(Array<StaticMeshData> mesh_data,
			 Array<GLuint> mesh_elems,
			 Texture* texture) {

    tex = *texture;
    RenderableEBO<StaticMeshData>::shader_vars
      = {Shader::POSITION, Shader::NORMAL, Shader::TEX_COORD};
    RenderableEBO<StaticMeshData>::is_dynamic = false;
    Renderable<StaticMeshData>::data = mesh_data;
    RenderableEBO<StaticMeshData>::elements = mesh_elems;
    RenderableEBO<StaticMeshData>::init(/*{Shader::POSITION,
					  Shader::NORMAL, Shader::TEX_COORD}, false*/);
      
  }

  void StaticMesh::render() {
    tex.use();
    RenderableEBO<StaticMeshData>::render();
  }

}
