#include "BonedMesh.hpp"

NAMESPACE {

  BonedMeshBase::BonedMeshBase(Array<BasicMeshData> static_data,
			       Array<u32> elems,
			       Texture* tex,
			       Array<BonedMeshData> bone_data,
			       Array<Bone> default_bones)
    : StaticMesh(static_data, elems, tex), b_data(bone_data),
    bones(default_bones) {

    Renderable::init();
    b_vbo.init();
    Renderable::vbo.bindArray(StaticMesh::data, false);
    Renderable::ebo.bindArray(StaticMesh::elements, false);
    Renderable::vao.registerVars({Shader::POSITION,
	  Shader::NORMAL, Shader::TEX_COORD});
    b_vbo.bindArray(data, false);
    Renderable::vao.registerVars({Shader::NUM_BONES,
	  Shader::BONE_INDEXES0,
	  Shader::BONE_INDEXES1,
	  Shader::BONE_WEIGHTS0,
	  Shader::BONE_WEIGHTS1});
    
  }

  BonedMesh::BonedMesh(BonedMeshBase* base_mesh)
    : base(base_mesh), bones(base_mesh->bones) {}

  void BonedMesh::render() {
    base->render();
  }

  BoundingObject BonedMesh::getBoundingObject() {
    return base->getBoundingObject();
  }
  
  BoundingObject BonedMesh::getPrimaryBoundingObject() {
    return base->getPrimaryBoundingObject();
  }
  
  BoundingObject BonedMesh::getPhysicalBoundingObject() {
    return base->getPhysicalBoundingObject();
  }

}
