#include "StaticMesh.hpp"

NAMESPACE {

  StaticMesh::StaticMesh(Array<BasicMeshData> mesh_data,
			 Array<u32> mesh_elems,
			 Texture* texture)
    : data(mesh_data), elements(mesh_elems),
    tex(texture), b_sphere(BOUNDING_SPHERE, mesh_data) {}

  void StaticMesh::init() {
    RenderableReg::init();
    /*for (BasicMeshData d : data) {
      Log::message(d.pos.toString());
      }*/
    RenderableReg::vbo.bindArray(data, false);
    RenderableReg::ebo.bindArray(elements, false);
    RenderableReg::vao.registerVars({Shader::POSITION,
	  Shader::NORMAL, Shader::TEX_COORD});
  }

  StaticMesh::~StaticMesh() {
    delete tex;
  }

  BoundingObject StaticMesh::getBoundingObject() {
    return b_sphere;
  }
  
  BoundingObject StaticMesh::getPhysicalBoundingObject() {
    return b_sphere;
  }

  BoundingObject StaticMesh::getPrimaryBoundingObject() {
    return b_sphere;
  }

  void StaticMesh::render() {
    tex->use();
    RenderableReg::render();
  }

}
