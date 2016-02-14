#pragma once

#include "Standard.hpp"
#include "Messageable.hpp"
#include "Pointable.hpp"
#include "Transform.hpp"
#include "BoundingObjectUnion.hpp"

NAMESPACE {

  struct Engine;
  struct Component;

  struct GameObject : BaseRTTI, Messageable, Transform {

    Engine* engine;
    Array<Pointer<Component>> components;

    BoundingObjectUnion tight_object;
    BoundingObjectUnion loose_object;

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
      for (Pointer<Component>& p : components) {
	if (typeId(p) == typeId<T>()) {
	  return (Pointer<T>) p;
	}
      }
      return NULL;
    }

    BoundingObject* getTightBoundingObject();
    BoundingObject* getLooseBoundingObject();
    
  };
  
}
