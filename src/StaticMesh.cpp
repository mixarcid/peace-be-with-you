#include "StaticMesh.hpp"

NAMESPACE {

  StaticMesh::StaticMesh(Array<StaticMeshData> mesh_data,
			 Array<GLuint> mesh_elems,
			 Texture* texture) {

    tex = texture;
    Renderable<StaticMeshData>::data = mesh_data;
    Renderable<StaticMeshData>::elements = mesh_elems;
    Renderable<StaticMeshData>::init({Shader::POSITION,
	  Shader::NORMAL, Shader::TEX_COORD}, false);
      
  }

  StaticMesh::~StaticMesh() {
    delete tex;
  }

  void StaticMesh::render() {
    tex->use();
    Renderable<StaticMeshData>::render();
  }

}
