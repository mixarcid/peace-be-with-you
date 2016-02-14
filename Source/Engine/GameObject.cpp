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
}
