#include "GameObject.hpp"

NAMESPACE {

  GameObject::GameObject(Engine* _engine, Vec3f pos)
    : engine(_engine), transform(pos) {}

  Vec3f GameObject::getTrans() {
    return transform.trans;
  }
  Quaternionf GameObject::getRot() {
    return transform.rot;
  }
  void GameObject::setTrans(Vec3f trans) {
    transform.trans = trans;
  }
  void GameObject::setRot(Quaternionf rot) {
    transform.rot = rot;
  }

  BoundingObject GameObject::getTightBoundingObject() {
    return BoundingObject();
  }
  BoundingObject GameObject::getLooseBoundingObject() {
    return BoundingObject();
  }
  BoundingAABB GameObject::getBoundingAABB() {
    return BoundingAABB(Vec3f(0,0,0), Vec3f(0,0,0));
  }
}
