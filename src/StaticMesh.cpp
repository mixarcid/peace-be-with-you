#include "StaticMesh.hpp"

NAMESPACE {

  StaticMesh::StaticMesh(Array<BasicMeshData> mesh_data,
			 Array<u32> mesh_elems,
			 Texture* texture)
    : data(mesh_data),
    elements(mesh_elems),
    tex(texture),
    b_sphere(BOUNDING_SPHERE, mesh_data),
    b_obb(BOUNDING_OBB, mesh_data) {}

  void StaticMesh::init() {
    RenderableReg::init();
    RenderableReg::vbo.bindArray(data, false);
    RenderableReg::ebo.bindArray(elements, false);
    RenderableReg::vao.registerVars({Shader::POSITION,
	  Shader::NORMAL, Shader::TEX_COORD});
  }

  BoundingObject StaticMesh::getBoundingObject() {
    return b_sphere;
  }
  
  BoundingObject StaticMesh::getPhysicalBoundingObject() {
    return b_sphere;
  }

  BoundingObject StaticMesh::getPrimaryBoundingObject() {
    return b_obb;
  }

  void StaticMesh::render(RenderContext c) {
    Shader::setFlags(SHADER_NO_FLAGS);
    tex->use();
    RenderableReg::render(c);
  }

}
