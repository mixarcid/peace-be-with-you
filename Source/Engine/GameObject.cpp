#include "GameObject.hpp"
#include "PhysicsComp.hpp"

NAMESPACE {

  GameObject::GameObject(Engine* _engine,
			 Vec3f trans,
			 Quaternionf rot)
    : Transform(trans, rot), engine(_engine) {}

  Transform GameObject::getTransform() {
    return Transform(this->trans, this->rot);
  }

  void GameObject::setTransform(Transform t) {
    this->trans = t.trans;
    this->rot = t.rot;
  }

  BoundingObject* GameObject::getTightBoundingObject() {
    return tight_object.getBoundingObject();
  }
  
  BoundingObject* GameObject::getLooseBoundingObject() {
    return loose_object.getBoundingObject();
  }

  ChildObject::ChildObject(Transform _diff,
			   Pointer<DynamicObject> _obj)
    : diff(_diff), obj(_obj) {}
  
  void DynamicObject::transRel(Vec3f trans) {
    transAbs(getTrans() + trans);
  } 
  
  void DynamicObject::transAbs(Vec3f trans) {
    Transform diff(trans - getTrans());
    this->trans = trans;
    getTightBoundingObject()->transform(diff);
    getLooseBoundingObject()->transform(diff);
    onMove();
  }
  
  void DynamicObject::rotRel(Quaternionf rot) {
    rotAbs(getRot() + rot);
  }
  
  void DynamicObject::rotAbs(Quaternionf rot) {
    Transform diff(Vec3f(0,0,0), rot*(getRot().conjugate()));
    this->rot = rot;
    getTightBoundingObject()->transform(diff);
    getLooseBoundingObject()->transform(diff);
    onMove();
  }
  
  void DynamicObject::onMove() {
    
    for (const ChildObject& t : children) {
      Transform trans(Transform::combine(getTransform(), t.diff));
      t.obj->setTransform(trans);
      t.obj->onMove();
    }
	
    Pointer<DynamicPhysicsComp> phys
      = getComponent<DynamicPhysicsComp>();
    if (phys) {
      phys->onMove(this);
    }
  }

  ChildObject* DynamicObject::addChild
    (Pointer<DynamicObject> child,
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
