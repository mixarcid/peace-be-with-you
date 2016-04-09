#include "StaticMesh.hpp"

NAMESPACE {

  const ShaderFlags StaticMesh::SHADER_FLAGS;
  
  StaticMesh::StaticMesh(Texture texture,
			 ShaderFlags flags)
    : RenderableReg(flags),
    tex(texture) {}

  void StaticMesh::init(Array<BasicMeshData>& data,
			Array<u32>& elements) {
    RenderableReg::init();
    this->ebo.bindArray(elements, false);
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
    return &b_sphere;
  }
  
  void StaticMesh::render(RenderContext c) {
    Shader::UNI_TEXTURE.registerTexture(tex);
    RenderableReg::render(c);
  }

}
