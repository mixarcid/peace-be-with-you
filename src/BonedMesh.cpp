#include "BonedMesh.hpp"
#include "FileIO.hpp"

NAMESPACE {

  BonedMeshData::BonedMeshData(u32 bone_num)
    : num_bones(bone_num) {
      memset(indexes, 0u, sizeof(indexes));
      memset(weights, 0.0f, sizeof(weights));
  }

  KeyFrame::KeyFrame(f32 frame_time, u32 num_bones)
    : time(frame_time) {
    bones.reserve(num_bones);
  }

  BonedAnimation::BonedAnimation(Array<KeyFrame> anim_keyframes)
    : playing(true),
    last_time(0.0f),
    last_keyframe(0),
    keyframes(anim_keyframes) {}
  
  void BonedAnimation::step(Array<Bone> bones, f32 dt) {
    
    if (!playing) return;
    
    f32 cur_time = last_time + dt;
    u32 cur_keyframe;
    
    for (u32 i = cur_keyframe; i < keyframes.length(); ++i) {
      if (cur_time >= keyframes[i].time) {
	if (i == keyframes.length()) {
	  playing = false;
	  return;
	}
	cur_keyframe = i+1;
	break;
      }
    }

    f32 h = ((cur_time - keyframes[last_keyframe].time)
	     /(cur_keyframe - keyframes[cur_keyframe].time));
    for (u32 i = 0; i < bones.length(); ++i) {
      bones[i] = Transform::interp(bones[i],
				   keyframes[cur_keyframe]
				   .bones[i],
				   h);
    }
  }

  BonedMeshBase::BonedMeshBase(Array<BasicMeshData> static_data,
			       Array<u32> elems,
			       Texture* tex,
			       Array<BonedMeshData> bone_data,
			       Array<Bone> default_bones,
			       HashMap<String,
			       BonedAnimation> mesh_animations)
    : StaticMesh(static_data, elems, tex),
    b_data(bone_data),
    bones(default_bones),
    animations(mesh_animations) {}

  void BonedMeshBase::init() {

    /*for (Bone b : bones) {
      Log::message(b.trans.toString());
      Log::message(b.rot.toString());
      }*/
    /*for (auto d : StaticMesh::data) {
      Log::message("Normal: " + d.norm.toString());
      Log::message("Abs: %f\n", d.norm.abs());
      }*/
    
    RenderableReg::init();
    b_vbo.init();

    RenderableReg::vbo.bindArray(StaticMesh::data, false);
    RenderableReg::vao.registerVars({Shader::POSITION,
	  Shader::NORMAL, Shader::TEX_COORD});

    b_vbo.bindArray(b_data, false);
    RenderableReg::vao.registerVars({Shader::NUM_BONES,
	  Shader::BONE_INDEXES0,
	  Shader::BONE_INDEXES1,
	  Shader::BONE_WEIGHTS0,
	  Shader::BONE_WEIGHTS1});

    RenderableReg::ebo.bindArray(StaticMesh::elements, false);
  }

  BonedMesh::BonedMesh(BonedMeshBase* base_mesh)
    : base(base_mesh),
    bones(base_mesh->bones) {}

  void BonedMesh::startAnimation(String name) {
    cur_animation = base->getAnimation(name);
  }

  void BonedMesh::render(RenderContext c) {
    cur_animation.step(c.dt);
    Shader::UNI_BONES.registerBufferData(bones);
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
