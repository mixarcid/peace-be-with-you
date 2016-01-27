#pragma once

#include "Standard.hpp"
#include "Messageable.hpp"
#include "Pointable.hpp"
#include "Transform.hpp"
#include "BoundingObject.hpp"

NAMESPACE {

  struct Engine;
  struct Component;
  
  struct GameObject : BaseRTTI, Pointable, Messageable {

    Engine* engine;
    Array<Pointer<Component>> components;
    Transform transform;

    GameObject(Engine* _engine, Vec3f pos);

    template <typename T>
    void addComponent(Pointer<T> component) {
      components.push_back((Pointer<Component>) component);
    }

    template <typename T>
    void addComponent(T* component) {
      components.push_back((Pointer<Component>) component);
    }
    
    template <typename T>
    Pointer<T> getComponent() {
      for (Pointer<Component> p : components) {
	if (typeId(p) == typeId<T>()) {
	  return (Pointer<T>) p;
	}
      }
      return NULL;
    }

    //for either graphics or physics
    virtual BoundingObject getTightBoundingObject();
    virtual BoundingObject getLooseBoundingObject();
    virtual BoundingAABB getBoundingAABB();

    Vec3f getTrans();
    Quaternionf getRot();
    void setTrans(Vec3f trans);
    void setRot(Quaternionf rot);
    
  };
  
}
