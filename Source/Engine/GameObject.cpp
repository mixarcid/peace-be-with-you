#include "GameObject.hpp"

NAMESPACE {

  GameObject::GameObject(Engine* _engine, Vec3f pos)
    : Transform(pos), engine(_engine) {}

  BoundingObject* GameObject::getTightBoundingObject() {
    return tight_object.getBoundingObject();
  }
  BoundingObject* GameObject::getLooseBoundingObject() {
    return loose_object.getBoundingObject();
  }

  /*void GameObject::transRel(Vec3f trans) {
    Transform::transRel(trans);
    onChange();
  }
  void GameObject::transAbs(Vec3f trans) {
    Transform::transAbs(trans);
    onChange();
  }
  void GameObject::rotRel(Quaternionf rot) {
    Transform::rotRel(rot);
    onChange();
  }
  void GameObject::rotAbs(Quaternionf rot) {
    Transform::rotAbs(rot);
    onChange();
  }
  void GameObject::onChange() {
    tight_object.getBoundingObject()->transform(*((Transform*)this));
    loose_object.getBoundingObject()->transform(*((Transform*)this));
    }*/
}
