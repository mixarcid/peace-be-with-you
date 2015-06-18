#include "Transform.hpp"
#include "Shader.hpp"

NAMESPACE {
  
  Transform::Transform() : scal(1,1,1) {}

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
    
  void Transform::scaleAbs(Vec3f scale_v) {
    scal = scale_v;
  }
  void Transform::scaleRel(Vec3f scale_v) {
    scal += scale_v;
  }

  Mat4f Transform::getMat() {
    Mat4f ret = Mat4f::makeScale(scal) *
      rot.getMat() * Mat4f::makeTranslate(trans);
    return ret;
  }
  
}
