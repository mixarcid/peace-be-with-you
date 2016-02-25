#include "Transform.hpp"

NAMESPACE {

  ChildTransform::ChildTransform(TransformBasic _diff,
				 Pointer<Transform> _obj)
    : diff(_diff), obj(_obj) {}

  ChildTransform* Transform::addChildTransform(Transform* child,
					       TransformBasic diff) {
    *child = Transform::combine(*this, diff);
    child->onChange();
    return child_transforms.push_back(ChildTransform(diff, child));
  }

  TransformBasic Transform::getBasicTransform() {
    return TransformBasic(trans, rot);
  }
  Vec3f Transform::getTrans() {
    return this->trans;
  }
  Quaternionf Transform::getRot() {
    return this->rot;
  }
  
  void Transform::transRel(Vec3f trans) {
    transAbs(getTrans() + trans);
  }
  void Transform::transAbs(Vec3f trans) {
    this->trans = trans;
    onChange();
  }
  
  void Transform::rotRel(Quaternionf rot) {
    rotAbs(getRot() + rot);
  }
  void Transform::rotAbs(Quaternionf rot) {
    this->rot = rot;
    onChange();
  }

  void Transform::onChange() {
    for (const ChildTransform& t : child_transforms) {
      *(t.obj) = Transform::combine(*this, t.diff);
      t.obj->onChange();
    }
  }

  void Transform::moveChildTransformAbs(ChildTransform* child,
			       TransformBasic diff) {
    child->diff = diff;
    *(child->obj) = Transform::combine(*this, child->diff);
    child->obj->onChange();
  }

  void Transform::moveChildTransformRel(ChildTransform* child,
			       TransformBasic diff) {
    moveChildTransformAbs
      (child, TransformBasic::combine(child->diff, diff));
  }

}
