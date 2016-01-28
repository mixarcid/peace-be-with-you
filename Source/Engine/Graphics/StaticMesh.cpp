#include "StaticMesh.hpp"

NAMESPACE {

  StaticMesh::StaticMesh(Texture* texture,
			 ShaderFlags _shader_flags)
    : RenderableReg(_shader_flags),
    tex(texture) {}

  void StaticMesh::init() {
    RenderableReg::init();
    RenderableReg::ebo.init();
    RenderableReg::ebo.bindArray(elements, false);
    RenderableReg::vbo.bindArray(data, false);
    RenderableReg::vao.registerVars({Shader::POSITION,
	  Shader::NORMAL,
	  Shader::TEX_COORD});
    b_sphere = BoundingObject(BOUNDING_SPHERE, data);
    b_obb = BoundingObject(BOUNDING_OBB, data);
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
