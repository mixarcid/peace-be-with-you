#include "Transform.hpp"
#include "Shader.hpp"

NAMESPACE {
  
  Transform::Transform(Vec3f translation, Quaternionf rotation)
    : trans(translation),
    //scal(scale),
    rot(rotation) {}

  void Transform::translateAbs(Vec3f trans_v) {
    trans = trans_v;
  }
  void Transform::translateRel(Vec3f trans_v) {
    trans += trans_v;
  }

  void Transform::rotateAbs(Quaternionf q) {
    rot = q.normalized();
  }
  void Transform::rotateRel(Quaternionf q) {
    rot *= q;
    //Log::message("??");
    rot.normalize();
  }
    
  /*void Transform::scaleAbs(Vec3f scale_v) {
    scal = scale_v;
  }
  void Transform::scaleRel(Vec3f scale_v) {
    scal += scale_v;
    }*/

  Mat4f Transform::getMat() {
    Mat4f ret = //Mat4f::makeScale(scal) *
      Mat4f::translate(trans)*rot.mat4();
    //Log::message(to_string(rot));
    //Log::message(to_string(rot.mat4()));
    return ret;
  }

  Transform Transform::combine(Transform a, Transform b) {
    Transform ret(a.trans + (a.rot * b.trans),
		  b.rot*a.rot);
    return ret;
  }

  Transform Transform::interp(Transform a, Transform b, f32 h) {
    return Transform(Vec3f::lerp(a.trans, b.trans, h),
		     Quaternionf::lerp(a.rot, b.rot, h));
  }
}
