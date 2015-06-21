#include "StaticMesh.hpp"

NAMESPACE {

  StaticMesh::StaticMesh(Array<BasicMeshData> mesh_data,
			 Array<GLuint> mesh_elems,
			 Texture* texture)
    : data(mesh_data), elements(mesh_elems),
    tex(texture), b_sphere(mesh_data) {
    
    Renderable::init();
    Renderable::vbo.bindArray(data, false);
    Renderable::ebo.bindArray(elements, false);
    Renderable::vao.registerVars({Shader::POSITION,
	  Shader::NORMAL, Shader::TEX_COORD});
      
  }

  StaticMesh::~StaticMesh() {
    delete tex;
  }

  BoundingObject* StaticMesh::getBoundingObject() {
    return (BoundingObject*) &b_sphere;
  }
  
  BoundingObject* StaticMesh::getPhysicalBoundingObject() {
    return (BoundingObject*) &b_sphere;
  }

  void StaticMesh::render() {
    tex->use();
    Renderable::render();
  }

}
