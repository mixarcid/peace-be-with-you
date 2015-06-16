#include "StaticMesh.hpp"

NAMESPACE {

  StaticMesh::StaticMesh(Array<StaticMeshData> mesh_data,
			 Array<GLuint> mesh_elems,
			 Texture* texture)
    : data(mesh_data), elements(mesh_elems), tex(texture) {

    Renderable::init();
    Renderable::vbo.bindArray(data, false);
    Renderable::ebo.bindArray(elements, false);
    Renderable::vao.registerVars({Shader::POSITION,
	  Shader::NORMAL, Shader::TEX_COORD});
      
  }

  StaticMesh::~StaticMesh() {
    delete tex;
  }

  void StaticMesh::render() {
    tex->use();
    Renderable::render();
  }

}
