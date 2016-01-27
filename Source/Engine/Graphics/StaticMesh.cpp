#include "StaticMesh.hpp"

NAMESPACE {

  StaticMesh::StaticMesh(Array<BasicMeshData> mesh_data,
			 Array<u32> mesh_elems,
			 Texture* texture,
			 ShaderFlags _shader_flags)
    : RenderableReg(_shader_flags),
    data(mesh_data),
    elements(mesh_elems),
    tex(texture),
    b_sphere(BOUNDING_SPHERE, mesh_data),
    b_obb(BOUNDING_OBB, mesh_data) {}

  void StaticMesh::init() {
    RenderableReg::init();
    RenderableReg::ebo.init();
    RenderableReg::ebo.bindArray(elements, false);
    RenderableReg::vbo.bindArray(data, false);
    RenderableReg::vao.registerVars({Shader::POSITION,
	  Shader::NORMAL,
	  Shader::TEX_COORD});
  }

  BoundingObject StaticMesh::getTightBoundingObject() {
    return b_obb;
  }
    
  BoundingObject StaticMesh::getLooseBoundingObject() {
    return b_sphere;
  }

  void StaticMesh::render(RenderContext c) {
    Shader::setFlags(RenderableComp::shader_flags);
    tex->use();
    RenderableReg::render(c);
  }

}
