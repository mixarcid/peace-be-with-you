#pragma once

#include "StaticMesh.hpp"
#include "Quaternion.hpp"
#include "Transform.hpp"

NAMESPACE {

  struct Bone : Transform {
    using Transform::Transform;
    void operator=(const Transform b);
  };

  struct BonedMeshData {
    u32 indexes[Shader::MAX_BONES_PER_VERTEX];
    f32 weights[Shader::MAX_BONES_PER_VERTEX];

    BonedMeshData();
  };

  struct KeyFrame {
    
    f32 time;
    Array<Bone> bones;

    KeyFrame(f32 frame_time, u32 num_bones);
    
  };

  struct BonedAnimationBase {
    Array<KeyFrame> keyframes;
    BonedAnimationBase(Array<KeyFrame>
		       anim_keyframes = {});
  };

  struct BonedAnimation {

    bool playing;
    float cur_time;
    u32 cur_keyframe;
    Array<KeyFrame> keyframes;

    BonedAnimation(Array<KeyFrame> anim_keyframes);
    BonedAnimation();
    //returns true if animation is finished
    void step(Array<Bone>* bones, float dt);
    
  };

  struct BonedMeshBase : StaticMesh {

    VBO b_vbo;
    Array<BonedMeshData> b_data;
    Array<Bone> bones; //default pose4
    HashMap<String, BonedAnimationBase> animations;

    BonedMeshBase(Array<BasicMeshData> static_data,
		  Array<u32> elems,
		  Texture* tex,
		  Array<BonedMeshData> bone_data,
		  Array<Bone> default_bones,
		  HashMap<String,
		  BonedAnimationBase> mesh_animations);
    void init();
    BonedAnimation getAnimation(String name);
  };

  struct BonedMesh : Renderable {

    BonedMeshBase* base;
    Array<Bone> bones;
    BonedAnimation cur_animation;

    BonedMesh();
    BonedMesh(BonedMeshBase* base_mesh);
    void startAnimation(String name);
    virtual void render(RenderContext c);
    virtual BoundingObject getTightBoundingObject();
    virtual BoundingObject getLooseBoundingObject();
    
  };
}
