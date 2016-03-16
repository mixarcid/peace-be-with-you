#pragma once

#include "Standard.hpp"
#include "Pointable.hpp"
#include "Transform.hpp"
#include "BoundingObjectUnion.hpp"
#include "Component.hpp"
#include "Grid.hpp"
#include "Message.hpp"

NAMESPACE {
  
  struct GameObject : Pointable {
    
    Array<Pointer<Component>> components;
    BoundingObjectUnion tight_object;
    BoundingObjectUnion loose_object;
    Transform transform;

    GameObject(Vec3f trans,
	       Quaternionf rot);
    ~GameObject() {}

    template <typename T>
    void addComponent(T* component) {
      components.emplace_back<Pointer<T>>(component);
    }
    
    template <typename T>
    Pointer<T>& getComponent() {
      for (Pointer<Component>& p : components) {
	if (p && typeId(p) == typeId<T>()) {
	  return (Pointer<T>&) p;
	}
      }
      return Pointer<T>::null;
    }

    Vec3f getTrans();
    Quaternionf getRot();
    Mat4f getMat();
    Transform getTransform();
    void setTransform(Transform t);
    
    BoundingObject* getTightBoundingObject();
    BoundingObject* getLooseBoundingObject();

    virtual void message(Message* m) {}
    
  };

  struct StaticObject : GameObject {
    StaticObject(Vec3f trans,
		 Quaternionf rot = Quaternionf());
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
		Pointer<DynamicObject>& _obj);
    
  };

  struct DynamicObject : GameObject {

    typedef Grid::Handle Handle;

    Array<ChildObject> children;
    Handle handle;

    DynamicObject(Vec3f trans,
		  Quaternionf rot = Quaternionf());
    
    void onMove();
    void transRel(Vec3f trans);
    void transAbs(Vec3f trans);
    void rotRel(Quaternionf rot);
    void rotAbs(Quaternionf rot);
    ChildObject* addChild(Pointer<DynamicObject>& child,
			  Transform diff);
    void moveChildAbs(ChildObject* child,
		      Transform diff);
    void moveChildRel(ChildObject* child,
		      Transform diff);
    
  };
  
}
