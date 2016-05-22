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

    typedef u64 IdSize;
    
    Array<Pointer<Component>> components;
    BoundingObjectUnion tight_object;
    BoundingObjectUnion loose_object;
    IdSize id;
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

    static IdSize cur_id;
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

  PEACE_ENUM(ChildObjectType, 8,
	     CHILD_OBJECT_REGULAR,
	     CHILD_OBJECT_TRANSLATE,
	     CHILD_OBJECT_TRANSLATE_NO_Z);

  struct ChildObject  {

    Pointer<DynamicObject> obj;
    Transform diff;
    ChildObjectType type;

    ChildObject(Transform _diff,
		Pointer<DynamicObject>& _obj,
		ChildObjectType _type);
    
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
			  Transform diff,
			  ChildObjectType type = CHILD_OBJECT_REGULAR);
    void moveChildAbs(ChildObject* child,
		      Transform diff);
    void moveChildRel(ChildObject* child,
		      Transform diff);
    
  };
  
}
