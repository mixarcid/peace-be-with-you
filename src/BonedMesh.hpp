#pragma once

#include "StaticMesh.hpp"
#include "Quaternion.hpp"
#include "Transform.hpp"

NAMESPACE {

  struct Bone : Transform {
    using Transform::Transform;
  };

  struct BonedMeshData {
    u32 num_bones;
    u32 indexes[Shader::MAX_BONES_PER_VERTEX];
    f32 weights[Shader::MAX_BONES_PER_VERTEX];
  };

  struct BonedMeshBase : StaticMesh {

    VBO b_vbo;
    Array<BonedMeshData> b_data;
    Array<Bone> bones; //default pose

    BonedMeshBase(Array<BasicMeshData> static_data,
		  Array<u32> elems,
		  Texture* tex,
		  Array<BonedMeshData> bone_data,
		  Array<Bone> default_bones);
  };

  struct BonedMesh : Renderable {

    BonedMeshBase* base;
    Array<Bone> bones;

    BonedMesh(BonedMeshBase* base_mesh);
    virtual void render();
    virtual BoundingObject getBoundingObject();
    virtual BoundingObject getPrimaryBoundingObject();
    virtual BoundingObject getPhysicalBoundingObject();
    
  };
}
