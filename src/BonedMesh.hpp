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

    BonedMeshData(u32 bone_num);
  };

  struct KeyFrame {
    
    f32 time;
    Array<Bone> bones;

    KeyFrame(f32 frame_time, u32 num_bones);
    
  };

  struct BonedAnimation {
    
    float current_time;
    u32 last_keyframe;
    Array<KeyFrame> keyframes;

    BonedAnimation(Array<KeyFrame> anim_keyframes);
    void step(Array<Bone> bones, float dt);
    
  };

  struct BonedMeshBase : StaticMesh {

    VBO b_vbo;
    Array<BonedMeshData> b_data;
    Array<Bone> bones; //default pose4
    HashMap<String, BonedAnimation> animations;

    BonedMeshBase(Array<BasicMeshData> static_data,
		  Array<u32> elems,
		  Texture* tex,
		  Array<BonedMeshData> bone_data,
		  Array<Bone> default_bones,
		  HashMap<String,
		  BonedAnimation> mesh_animations);
    void init();
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
