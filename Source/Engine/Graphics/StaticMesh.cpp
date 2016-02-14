#include "StaticMesh.hpp"

NAMESPACE {

  StaticMesh::StaticMesh(Texture texture,
			 ShaderFlags _shader_flags)
    : RenderableReg(_shader_flags),
    tex(texture) {}

  void StaticMesh::init() {
    RenderableReg::init();
    this->ebo.bindArray(elements, false);
    this->vbo.bindArray(data, false);
    this->vao.registerVars({Shader::POSITION,
	  Shader::NORMAL,
	  Shader::TEX_COORD});
    b_sphere = BoundingSphere(data);
    b_obb = BoundingOBB(data);
  }

  void StaticMesh::init(EBO ebo) {
    RenderableReg::init(VBO(), VAO(), ebo);
    this->vbo.bindArray(data, false);
    this->vao.registerVars({Shader::POSITION,
	  Shader::NORMAL,
	  Shader::TEX_COORD});
    b_sphere = BoundingSphere(data);
    b_obb = BoundingOBB(data);
  }

  BoundingObject* StaticMesh::getTightBoundingObject() {
    return &b_obb;
  }
    
  BoundingObject* StaticMesh::getLooseBoundingObject() {
    return &b_obb;
  }
  
  void StaticMesh::render(RenderContext c) {
    Shader::setFlags(RenderableComp::shader_flags);
    Shader::UNI_TEXTURE.registerTexture(tex);
    RenderableReg::render(c);
  }

}
