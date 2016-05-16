#pragma once

#include "StaticMesh.hpp"
#include "Quaternion.hpp"
#include "Transform.hpp"

NAMESPACE {

  struct Bone : TransformPad {
    using TransformPad::TransformPad;
    void operator=(const TransformPad b);
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
    BonedAnimationBase() {}
  };

  PEACE_DEFINE_BITFLAGS(BonedAnimationFlags, 8,
			ANIMATION_NO_FLAGS = 0x00,
			ANIMATION_PLAYING = 0x01,
			ANIMATION_LOOP = 0x02);

  struct BonedAnimation {

    BonedAnimationFlags flags;
    float cur_time;
    u32 cur_keyframe;
    Array<KeyFrame> keyframes;

    BonedAnimation(Array<KeyFrame> anim_keyframes);
    BonedAnimation();
    void step(Array<Bone>* bones, float dt);
  };

  struct BonedMeshBase : StaticMesh {

    VBO b_vbo;
    Array<Bone> bones; //default pose
    HashMap<String, BonedAnimationBase> animations;

    BonedMeshBase(Texture tex);
    void init(Array<BasicMeshData>& mesh_data,
	      Array<BonedMeshData>& bone_data,
	      Array<u32>& elements);
    BonedAnimation getAnimation(String name);
    
  };

  struct BonedMesh : RenderableComp {

    BonedMeshBase* base;
    Array<Bone> bones;
    BonedAnimation cur_animation;
    String cur_animation_name;

    BonedMesh();
    BonedMesh(BonedMeshBase* base_mesh);
    void startAnimation(String name);
    void loopAnimation(String name);
    void stopAnimation();
    virtual void render(RenderContext c);
    BoundingObject* getTightBoundingObject();
    BoundingObject* getLooseBoundingObject();

    const static ShaderFlags SHADER_FLAGS
    = StaticMesh::SHADER_FLAGS | SHADER_SKELETAL;
    
  };
}
