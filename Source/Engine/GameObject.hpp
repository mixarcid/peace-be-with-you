#pragma once

#include "Standard.hpp"
#include "Messageable.hpp"
#include "Pointable.hpp"
#include "Transform.hpp"
#include "BoundingObjectUnion.hpp"
#include "Component.hpp"

NAMESPACE {

  struct Engine;

  struct GameObject : Pointable, Transform, Messageable {

    Engine* engine;
    Array<Pointer<Component>> components;

    BoundingObjectUnion tight_object;
    BoundingObjectUnion loose_object;

    GameObject(Engine* _engine,
	       Vec3f trans,
	       Quaternionf rot = Quaternionf());
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

    Transform getTransform();
    void setTransform(Transform t);
    
    BoundingObject* getTightBoundingObject();
    BoundingObject* getLooseBoundingObject();

  };

  struct StaticObject : GameObject {
    using GameObject::GameObject;
    inline void transRel(Vec3f trans) {}
    inline void transAbs(Vec3f trans) {}
    inline void rotRel(Quaternionf rot) {}
    inline void rotAbs(Quaternionf rot) {}
  };

  struct DynamicObject;

  struct ChildObject  {
    
    Transform diff;
    Pointer<DynamicObject> obj;

    ChildObject(Transform _diff,
		   Pointer<DynamicObject> _obj);
    
  };

  struct DynamicObject : GameObject {

    Array<ChildObject> children;

    using GameObject::GameObject;
    
    void onMove();
    void transRel(Vec3f trans);
    void transAbs(Vec3f trans);
    void rotRel(Quaternionf rot);
    void rotAbs(Quaternionf rot);
    ChildObject* addChild(Pointer<DynamicObject> child,
			  Transform diff);
    void moveChildAbs(ChildObject* child,
		      Transform diff);
    void moveChildRel(ChildObject* child,
		      Transform diff);
    
  };

  template <typename T>
    struct StaticComponentPair {

    Pointer<StaticObject> obj;
    Pointer<T> comp;

    StaticComponentPair(Pointer<StaticObject> _obj,
			Pointer<T> _comp)
      : obj(_obj), comp(_comp) {}
    
  };

  template <typename T>
    struct StaticComponentPairP : Pointable, StaticComponentPair<T> {
    using StaticComponentPair<T>::StaticComponentPair;
    void _on_move() {
      this->comp._on_move();
      this->obj._on_move();
      Pointable::_on_move();
    }
  };

  template <typename T>
    struct DynamicComponentPair {

    Pointer<DynamicObject> obj;
    Pointer<T> comp;

    DynamicComponentPair(Pointer<DynamicObject> _obj,
			 Pointer<T> _comp)
      : obj(_obj), comp(_comp) {}
    
  };

  template <typename T>
    struct DynamicComponentPairP : Pointable, DynamicComponentPair<T> {
    using DynamicComponentPair<T>::DynamicComponentPair;
    void _on_move() {
      this->comp._on_move();
      this->obj._on_move();
      Pointable::_on_move();
    }
  };
  
}
