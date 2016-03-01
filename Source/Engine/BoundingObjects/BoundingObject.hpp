#pragma once

#include "RenderableComp.hpp"
#include "Transform.hpp"

NAMESPACE {

  struct Manifold;
  struct BoundingObject;
  
  /*typedef function<bool(BoundingObject*, BoundingObject*)>
    TestCollisionFunc;
  typedef function<bool(BoundingObject*, BoundingObject*, Manifold*)>
  ManifoldFunc;*/

  typedef bool (*TestCollisionFunc) (BoundingObject*, BoundingObject*);
  typedef bool (*ManifoldFunc) (BoundingObject*, BoundingObject*, Manifold*);

  struct BoundingObject {

    PEACE_ENUM(BoundingObjectType,
	       AABB,
	       SPHERE,
	       OBB,
	       GROUND,
	       FRUSTUM,
	       NONE,
	       LAST);
    typedef BoundingObjectType Type;
    
    Type type;
    
    BoundingObject(Type _type = NONE);
    virtual ~BoundingObject() {}
    bool isContainedIn(BoundingObject* b);
    bool intersects(BoundingObject* b);
    bool intersects(BoundingObject* b, Manifold* man);

    virtual f32 getVolume();
    virtual f32 getInertia(f32 mass);
    //return value must be deleted manually
    virtual BoundingObject* transform(TransformBasic t);
    virtual void render(RenderContext c) {}

    static TestCollisionFunc
    contained_in_functions[LAST][LAST];
    static TestCollisionFunc
    collide_functions[LAST][LAST];
    static ManifoldFunc
    manifold_functions[LAST][LAST];

  };

  struct DefineContainedInFunc {
    DefineContainedInFunc(BoundingObject::Type a,
			  BoundingObject::Type b,
			  TestCollisionFunc func);
  };

  struct DefineCollideFunc {
    DefineCollideFunc(BoundingObject::Type a,
		      BoundingObject::Type b,
		      TestCollisionFunc func);
  };

  struct DefineManifoldFunc {
    DefineManifoldFunc(BoundingObject::Type a,
		       BoundingObject::Type b,
		       ManifoldFunc func);
  };

#define CONTAINED_IN_FUNC(a_type, b_type, ...)				\
  DefineContainedInFunc contained_in##a_type##b_type			\
    (BoundingObject::a_type,						\
     BoundingObject::b_type, [] (BoundingObject* oa,			\
				 BoundingObject* ob)->bool {		\
      __VA_ARGS__							\
	});
  
#define COLLIDE_FUNC(a_type, b_type, ...)				\
  DefineCollideFunc collide##a_type##b_type				\
    (BoundingObject::a_type,						\
     BoundingObject::b_type, [] (BoundingObject* oa,			\
				 BoundingObject* ob)->bool {		\
      __VA_ARGS__							\
	});
  
#define MANIFOLD_FUNC(a_type, b_type, ...)				\
  DefineManifoldFunc manifold##a_type##b_type				\
    (BoundingObject::a_type,						\
     BoundingObject::b_type, [] (BoundingObject* oa, BoundingObject* ob, \
				 Manifold* man)->bool {			\
      __VA_ARGS__							\
	});
  

  template <BoundingObject::Type T>
    struct BoundingObjectBase : BoundingObject {

    BoundingObjectBase() : BoundingObject(T) {}
    
  };

}
