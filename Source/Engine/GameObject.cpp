#include "GameObject.hpp"
#include "PhysicsComp.hpp"
#include "Engine.hpp"

NAMESPACE {

  GameObject::GameObject(Vec3f trans,
			 Quaternionf rot)
    : transform(trans, rot) {}

  Transform GameObject::getTransform() {
    return transform;
  }

  void GameObject::setTransform(Transform t) {
    transform = t;
  }

  Vec3f GameObject::getTrans() {
    return transform.getTrans();
  }
  
  Quaternionf GameObject::getRot() {
    return transform.getRot();
  }

  Mat4f GameObject::getMat() {
    return transform.getMat();
  }

  BoundingObject* GameObject::getTightBoundingObject() {
    return tight_object.getBoundingObject();
  }
  
  BoundingObject* GameObject::getLooseBoundingObject() {
    return loose_object.getBoundingObject();
  }

  StaticObject::StaticObject(Vec3f trans,
			     Quaternionf rot)
    : GameObject(trans, rot) {}
  

  ChildObject::ChildObject(Transform _diff,
			   Pointer<DynamicObject>& _obj)
    : diff(_diff), obj(_obj) {}

  DynamicObject::DynamicObject(Vec3f trans,
			       Quaternionf rot)
    : GameObject(trans, rot) {}
    
  void DynamicObject::transRel(Vec3f trans) {
    transAbs(getTrans() + trans);
  } 
  
  void DynamicObject::transAbs(Vec3f trans) {
    Transform diff(trans - getTrans());
    transform.trans = trans;
    getTightBoundingObject()->transform(diff);
    getLooseBoundingObject()->transform(diff);
    onMove();
  }
  
  void DynamicObject::rotRel(Quaternionf rot) {
    rotAbs(getRot() + rot);
  }
  
  void DynamicObject::rotAbs(Quaternionf rot) {
    Transform diff(Vec3f(0,0,0), rot*(getRot().conjugate()));
    transform.rot = rot;
    getTightBoundingObject()->transform(diff);
    getLooseBoundingObject()->transform(diff);
    onMove();
  }
  
  void DynamicObject::onMove() {

    Pointer<DynamicObject> obj(this);
    Engine::registerMove(obj);
    
    for (const ChildObject& t : children) {
      Transform trans(Transform::combine(getTransform(), t.diff));
      t.obj->setTransform(trans);
      t.obj->onMove();
    }
	
    Pointer<DynamicPhysicsComp>& phys
      = getComponent<DynamicPhysicsComp>();
    if (phys) {
      phys->onMove(obj);
    }
  }

  ChildObject* DynamicObject::addChild
    (Pointer<DynamicObject>& child,
     Transform diff) {
    child->setTransform(Transform::combine(getTransform(), diff));
    child->onMove();
    return children.push_back(ChildObject(diff, child));
  }
  

  void DynamicObject::moveChildAbs(ChildObject* child,
				   Transform diff) {
    child->diff = diff;
    Transform comb = Transform::combine(getTransform(), child->diff);
    child->obj->setTransform(comb);
    child->obj->onMove();
  }
  
  void DynamicObject::moveChildRel(ChildObject* child,
				   Transform diff) {
    moveChildAbs
      (child, Transform::combine(child->diff, diff));
  }

  
}
