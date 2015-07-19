#include "BonedMesh.hpp"
#include "FileIO.hpp"

NAMESPACE {

  BonedMeshData::BonedMeshData(u32 bone_num)
    : num_bones(bone_num) {
      memset(indexes, 0u, sizeof(indexes));
      memset(weights, 0.0f, sizeof(weights));
  }

  KeyFrame::KeyFrame(f32 frame_time, u32 num_bones)
    : time(frame_time), bones(num_bones) {}

  BonedAnimation::BonedAnimation(Array<KeyFrame> anim_keyframes)
    : current_time(0.0),
    last_keyframe(0),
    keyframes(anim_keyframes) {}
  
  void BonedAnimation::step(Array<Bone> bones, float dt) {

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
      Log::message(b.rot.toString());
    }

    for (BonedMeshData d : b_data) {
      Log::message("#Bones: " + to_string(d.num_bones)
		   + ", Index0: " + to_string(d.indexes[0])
		   + ", Weight0: " + to_string(d.weights[0]));
      f32 w = 0;
      for (u32 n=0; n<8; ++n) {
	w += d.weights[n];
      }
      Log::message("Weight: %f", w);
      }*/

    /*for (u32 n = 0; n < b_data.size(); ++n) {
      b_data[n].weights[0] = 1.0;
      }*/

    //memset(&b_data[0], 1.0f, b_data.size()*sizeof(BonedMeshData));
    
    Renderable::init();
    b_vbo.init();

    Renderable::vbo.bindArray(StaticMesh::data, false);
    Renderable::vao.registerVars({Shader::POSITION,
	  Shader::NORMAL, Shader::TEX_COORD});

    b_vbo.bindArray(b_data, false);
    Renderable::vao.registerVars({Shader::NUM_BONES,
	  Shader::BONE_INDEXES0,
	  Shader::BONE_INDEXES1,
	  Shader::BONE_WEIGHTS0,
	  Shader::BONE_WEIGHTS1});

    Renderable::ebo.bindArray(StaticMesh::elements, false);
  }

  BonedMesh::BonedMesh(BonedMeshBase* base_mesh)
    : base(base_mesh),
    bones(base_mesh->bones) {}

  void BonedMesh::render() {
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
