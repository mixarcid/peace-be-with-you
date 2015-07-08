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
    rot = q.getUnit();
  }
  void Transform::rotateRel(Quaternionf q) {
    rot *= q;
    rot.makeUnit();
  }
    
  /*void Transform::scaleAbs(Vec3f scale_v) {
    scal = scale_v;
  }
  void Transform::scaleRel(Vec3f scale_v) {
    scal += scale_v;
    }*/

  Mat4f Transform::getMat() {
    Mat4f ret = //Mat4f::makeScale(scal) *
      rot.getMat() * Mat4f::makeTranslate(trans);
    return ret;
  }

  Transform Transform::combine(Transform a, Transform b) {
    Transform ret(a.trans + (a.rot * b.trans),
		  b.rot*a.rot);
    return ret;
  }
  
}
