#pragma once

#include "Transform.hpp"

NAMESPACE {

  struct Manifold;
  struct BoundingObject;

  enum BoundingObjectType {
    BOUNDING_AABB,
    BOUNDING_SPHERE,
    BOUNDING_OBB,
    BOUNDING_GROUND,
    BOUNDING_NONE,
    BOUNDING_LAST
  };
  
  typedef function<bool(BoundingObject*, BoundingObject*)>
    TestCollisionFunc;
  typedef function<bool(BoundingObject*, BoundingObject*, Manifold*)>
    ManifoldFunc;

  struct BoundingObject {
    
    BoundingObjectType type;
    
    BoundingObject(BoundingObjectType _type = BOUNDING_NONE);
    bool isContainedIn(BoundingObject* b);
    bool intersects(BoundingObject* b);
    bool intersects(BoundingObject* b, Manifold* man);

    virtual f32 getVolume();
    virtual f32 getInertia(f32 mass);
    virtual void transform(Transform t);

    static TestCollisionFunc
    contained_in_functions[BOUNDING_LAST][BOUNDING_LAST];
    static TestCollisionFunc
    collide_functions[BOUNDING_LAST][BOUNDING_LAST];
    static ManifoldFunc
    manifold_functions[BOUNDING_LAST][BOUNDING_LAST];

  };

  struct DefineContainedInFunc {
    DefineContainedInFunc(BoundingObjectType a,
			  BoundingObjectType b,
			  TestCollisionFunc func);
  };

  struct DefineCollideFunc {
    DefineCollideFunc(BoundingObjectType a,
		      BoundingObjectType b,
		      TestCollisionFunc func);
  };

  struct DefineManifoldFunc {
    DefineManifoldFunc(BoundingObjectType a,
		       BoundingObjectType b,
		       ManifoldFunc func);
  };

#define CONTAINED_IN_FUNC(a_type, b_type, ...)				\
  DefineContainedInFunc contained_in##a_type##b_type			\
    (a_type, b_type, [] (BoundingObject* oa,				\
			 BoundingObject* ob)->bool {			\
      __VA_ARGS__							\
	});
  
#define COLLIDE_FUNC(a_type, b_type, ...)				\
  DefineCollideFunc collide##a_type##b_type				\
    (a_type, b_type, [] (BoundingObject* oa,				\
			 BoundingObject* ob)->bool {			\
      __VA_ARGS__							\
	});
  
#define MANIFOLD_FUNC(a_type, b_type, ...)				\
  DefineManifoldFunc manifold##a_type##b_type				\
    (a_type, b_type, [] (BoundingObject* oa, BoundingObject* ob,	\
			 Manifold* man)->bool {				\
      __VA_ARGS__							\
	});
  

  template <BoundingObjectType Type>
    struct BoundingObjectBase : BoundingObject {

    BoundingObjectBase() : BoundingObject(Type) {}
    
  };

}
