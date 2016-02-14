#include "Transform.hpp"

NAMESPACE {

  void Transform::addTransformChild(Transform* child,
				    Transform initial_diff) {
    *child = Transform::combine(*child, initial_diff);
    child_transforms.push_back(Pointer<Transform>(child));
  }

  Vec3f Transform::getTrans() {
    return this->trans;
  }
  Quaternionf Transform::getRot() {
    return this->rot;
  }
  void Transform::transRel(Vec3f trans) {
    this->trans += trans;
    for (const Pointer<Transform> child : child_transforms) {
      child->transRel(trans);
    }
  }
  void Transform::transAbs(Vec3f trans) {
    this->trans = trans;
  }
  void Transform::rotRel(Quaternionf rot) {
    this->rot *= rot;
    for (const Pointer<Transform> child : child_transforms) {
      child->rotRel(rot);
    }
  }
  void Transform::rotAbs(Quaternionf rot) {
    this->rot = rot;
  }

}
