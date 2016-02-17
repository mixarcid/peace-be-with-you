#include "BonedMesh.hpp"
#include "FileIO.hpp"

NAMESPACE {

  void Bone::operator=(const TransformPad b) {
    memcpy(this, &b, sizeof(TransformPad));
  }

  BonedMeshData::BonedMeshData() {
      memset(indexes, 0u, sizeof(indexes));
      memset(weights, 0.0f, sizeof(weights));
  }

  KeyFrame::KeyFrame(f32 frame_time, u32 num_bones)
    : time(frame_time) {
    bones.reserve(num_bones);
  }
  
  BonedAnimation::BonedAnimation(Array<KeyFrame> anim_keyframes)
    : playing(true),
    cur_time(0.0f),
    cur_keyframe(1),
    keyframes(anim_keyframes) {}

  BonedAnimation::BonedAnimation()
    : playing(false),
    cur_time(0.0f),
    cur_keyframe(1),
    keyframes({}) {}

  void BonedAnimation::step(Array<Bone>* bones, f32 dt) {
    
    if (!playing) return;
    
    cur_time += dt;

    for (u32 i = cur_keyframe; i < keyframes.size(); ++i) {
      if (cur_time >= keyframes[i].time) {
	if (i == keyframes.size()-1) {
	  playing = false;
	  return;
	}
	cur_keyframe = i+1;
	break;
      }
    }
    debugAssert(cur_keyframe > 0,
		"Current keyframe should be greater than 0");
    f32 h;
    h = (((cur_time - keyframes[cur_keyframe-1].time))
	 /(keyframes[cur_keyframe].time - keyframes[cur_keyframe-1].time));
    if (h > 1.0f) {
      h = 1.0f;
    }
    
    for (u32 i = 0; i < bones->size(); ++i) {
      (*bones)[i] = Transform::interp(keyframes[cur_keyframe - 1]
				      .bones[i],
				      keyframes[cur_keyframe]
				      .bones[i],
				      h);
    }
    //Log::message("h: %f, ck: %u, t: %f", h, cur_keyframe, cur_time);
  }

  BonedMeshBase::BonedMeshBase(Texture tex)
    : StaticMesh(tex, SHADER_SKELETAL) {}

  void BonedMeshBase::init(Array<BasicMeshData>& mesh_data,
			   Array<BonedMeshData>& bone_data,
			   Array<u32>& elements) {
    
    RenderableReg::init();
    b_vbo.init();

    RenderableReg::vbo.bindArray(mesh_data, false);
    RenderableReg::vao.registerVars({Shader::POSITION,
	  Shader::NORMAL, Shader::TEX_COORD});

    b_vbo.bindArray(bone_data, false);
    RenderableReg::vao.registerVars({Shader::BONE_INDEXES0,
	  Shader::BONE_WEIGHTS0});

    RenderableReg::ebo.bindArray(elements, false);
  }

  BonedAnimation BonedMeshBase::getAnimation(String name) {
    return BonedAnimation(animations[name].keyframes);
  }

  BonedMesh::BonedMesh()
    : RenderableComp(SHADER_NO_FLAGS) {}


  BonedMesh::BonedMesh(BonedMeshBase* base_mesh)
    : RenderableComp(base_mesh->shader_flags),
    base(base_mesh),
    bones(base_mesh->bones) {}

  void BonedMesh::startAnimation(String name) {
    cur_animation = base->getAnimation(name);
  }

  void BonedMesh::render(RenderContext c) {
    cur_animation.step(&bones, c.dt);
    Shader::UNI_BONES.registerArray(bones);
    base->render(c);
  }

  BoundingObject* BonedMesh::getTightBoundingObject() {
    return base->getTightBoundingObject();
  }
  
  BoundingObject* BonedMesh::getLooseBoundingObject() {
    return base->getLooseBoundingObject();
  }

}
