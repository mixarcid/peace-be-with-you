#include "BoundingObject.hpp"

NAMESPACE {

  TestCollisionFunc
    BoundingObject::contained_in_functions[BOUNDING_LAST][BOUNDING_LAST];
  TestCollisionFunc
    BoundingObject::collide_functions[BOUNDING_LAST][BOUNDING_LAST];
  ManifoldFunc
    BoundingObject::manifold_functions[BOUNDING_LAST][BOUNDING_LAST];

  DefineContainedInFunc::DefineContainedInFunc(BoundingObjectType a,
					       BoundingObjectType b,
					       TestCollisionFunc func) {
    BoundingObject::contained_in_functions[a][b] = func;
  }

  DefineCollideFunc::DefineCollideFunc(BoundingObjectType a,
				       BoundingObjectType b,
				       TestCollisionFunc func) {
    BoundingObject::collide_functions[a][b] = func;
    BoundingObject::collide_functions[b][a] = func;
  }

  DefineManifoldFunc::DefineManifoldFunc(BoundingObjectType a,
					 BoundingObjectType b,
					 ManifoldFunc func) {
    BoundingObject::manifold_functions[a][b]  = func;
  }

  BoundingObject::BoundingObject(BoundingObjectType _type) : type(_type) {}

  bool BoundingObject::isContainedIn(BoundingObject* b) {
    return contained_in_functions[type][b->type](this, b);
  }
  
  bool BoundingObject::intersects(BoundingObject* b) {
    return collide_functions[type][b->type](this, b);
  }
  
  bool BoundingObject::intersects(BoundingObject* b, Manifold* man) {
    return manifold_functions[type][b->type](this, b, man);
  }

  f32 BoundingObject::getVolume() {
    return 0;
  }

  f32 BoundingObject::getInertia(f32 mass) {
    return 0;
  }
  
  void BoundingObject::transform(Transform t) {}
  
}
