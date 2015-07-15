#pragma once

#include "StaticMesh.hpp"
#include "Quaternion.hpp"
#include "Transform.hpp"

NAMESPACE {

  struct Bone : Transform {
    using Transform::Transform;
  };

  struct __attribute__((packed)) BonedMeshData {
    u32 num_bones;
    u32 indexes[Shader::MAX_BONES_PER_VERTEX];
    f32 weights[Shader::MAX_BONES_PER_VERTEX];
  };

  struct KeyFrame {
    
    float time;
    Array<Bone> bones;
    
  };

  struct Animation {
    
    float current_time;
    u32 last_keyframe;
    Array<KeyFrame> keyframes;

    Animation(Array<KeyFrame> anim_keyframes);
    void step(Array<Bone> bones, float dt);
    
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
    void init();
  };

  struct BonedMesh : Renderable {

    BonedMeshBase* base;
    Array<Bone> bones;
    HashMap<String, Animation> animations;

    BonedMesh(BonedMeshBase* base_mesh,
	      HashMap<String, Animation> mesh_animations);
    virtual void render();
    virtual BoundingObject getBoundingObject();
    virtual BoundingObject getPrimaryBoundingObject();
    virtual BoundingObject getPhysicalBoundingObject();
    
  };
}
