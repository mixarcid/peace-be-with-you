#pragma once

#include "Standard.hpp"
#include "Messageable.hpp"
#include "Pointable.hpp"
#include "Transform.hpp"
#include "BoundingObjectUnion.hpp"
#include "Component.hpp"

NAMESPACE {

  struct Engine;

  struct GameObject : Transform, Messageable {

    Engine* engine;
    Array<Pointer<Component>> components;

    BoundingObjectUnion tight_object;
    BoundingObjectUnion loose_object;

    GameObject(Engine* _engine, Vec3f pos);
    ~GameObject() {}

    template <typename T>
    void addComponent(Pointer<T> component) {
      components.push_back((Pointer<Component>) component);
    }

    template <typename T>
    void addComponent(T* component) {
      components.push_back(Pointer<Component>(component));
    }
    
    template <typename T>
    Pointer<T> getComponent() {
      for (Pointer<Component>& p : components) {
	if (p && typeId(p) == typeId<T>()) {
	  return (Pointer<T>) p;
	}
      }
      return NULL;
    }

    BoundingObject* getTightBoundingObject();
    BoundingObject* getLooseBoundingObject();

    /*void transRel(Vec3f trans);
    void transAbs(Vec3f trans);
    void rotRel(Quaternionf rot);
    void rotAbs(Quaternionf rot);
    void onChange();*/
    
  };

  template <typename T>
    struct ComponentPair {

    Pointer<GameObject> obj;
    Pointer<T> comp;

    ComponentPair(Pointer<GameObject> _obj,
		  Pointer<T> _comp)
      : obj(_obj), comp(_comp) {}
    
  };
  
}
