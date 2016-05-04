#include "BoundingObject.hpp"
#include "BoundingObjectUnion.hpp"
#include "Manifold.hpp"

NAMESPACE {

  TestCollisionFunc
    BoundingObject::contained_in_functions[BoundingObject::LAST][BoundingObject::LAST];
  TestCollisionFunc
    BoundingObject::collide_functions[BoundingObject::LAST][BoundingObject::LAST];
  ManifoldFunc
    BoundingObject::manifold_functions[BoundingObject::LAST][BoundingObject::LAST];

  DefineContainedInFunc::DefineContainedInFunc(BoundingObject::Type a,
					       BoundingObject::Type b,
					       TestCollisionFunc func) {
    BoundingObject::contained_in_functions[a][b] = func;
  }

  DefineCollideFunc::DefineCollideFunc(BoundingObject::Type a,
				       BoundingObject::Type b,
				       TestCollisionFunc func) {
    BoundingObject::collide_functions[a][b] = func;
    if (a != b) {
      BoundingObject::collide_functions[b][a] = ([](BoundingObject* oa, BoundingObject* ob) -> bool {
	  return BoundingObject::collide_functions[ob->type][oa->type](ob, oa);
	});
    }
  }

  DefineManifoldFunc::DefineManifoldFunc(BoundingObject::Type a,
					 BoundingObject::Type b,
					 ManifoldFunc func) {
    BoundingObject::manifold_functions[a][b] = func;
    if (a != b) {
      BoundingObject::manifold_functions[b][a] = ([](BoundingObject* oa, BoundingObject* ob, Manifold* m) -> bool {
	  if (!BoundingObject::manifold_functions[ob->type][oa->type](ob, oa, m)) {
	    return false;
	  } else {
	    m->normal = - m->normal;
	    return true;
	  } 
	});
    }
  }

  BoundingObject::BoundingObject(BoundingObject::Type _type)
    : type(_type) {}

  bool BoundingObject::isContainedIn(BoundingObject* b) {
    return contained_in_functions[type][b->type](this, b);
  }
  
  bool BoundingObject::intersects(BoundingObject* b) {
    return collide_functions[type][b->type](this, b);
  }
  
  bool BoundingObject::intersects(BoundingObject* b, Manifold* man) {
    return manifold_functions[type][b->type](this, b, man);
  }

  Vec3f BoundingObject::getCenter() {
    switch(this->type) {
      FOR(entry in getEnumEntries("BoundingObjectType")[:-3]);
    case BoundingObject::$(entry):
      return ((Bounding$(className(entry))*)this)->getCenter();
      END_FOR;
    case BoundingObject::NONE:
    case BoundingObject::ALL:
    case BoundingObject::LAST:
      return Vec3f(0,0,0);
    }
  }
  
  f32 BoundingObject::getVolume() {
    switch(this->type) {
      FOR(entry in getEnumEntries("BoundingObjectType")[:-3]);
    case BoundingObject::$(entry):
      return ((Bounding$(className(entry))*)this)->getVolume();
      END_FOR;
    case BoundingObject::NONE:
    case BoundingObject::ALL:
    case BoundingObject::LAST:
      return 0;
    }
  }

  f32 BoundingObject::getInertia(f32 mass) {
    switch(this->type) {
      FOR(entry in getEnumEntries("BoundingObjectType")[:-3]);
    case BoundingObject::$(entry):
      return ((Bounding$(className(entry))*)this)->getInertia(mass);
      END_FOR;
    case BoundingObject::NONE:
    case BoundingObject::ALL:
    case BoundingObject::LAST:
      return 0;
    }
  }
 
  void BoundingObject::transform(Transform t) {
    switch(this->type) {
      FOR(entry in getEnumEntries("BoundingObjectType")[:-3]);
    case BoundingObject::$(entry):
      ((Bounding$(className(entry))*)this)->transform(t);
      break;
      END_FOR;
    case BoundingObject::NONE:
    case BoundingObject::ALL:
    case BoundingObject::LAST:
      break;
    }
  }

  Pointer<RenderableComp> BoundingObject::getRenderable(Mat4f* model) {
    switch(this->type) {
      FOR(entry in getEnumEntries("BoundingObjectType")[:-3]);
    case BoundingObject::$(entry):
      return ((Bounding$(className(entry))*)this)->getRenderable(model);
      END_FOR;
    case BoundingObject::NONE:
    case BoundingObject::ALL:
    case BoundingObject::LAST:
      return Pointer<RenderableComp>::null;
    }
  }

  $for(entry in getEnumEntries("BoundingObjectType")[:-1]);
  COLLIDE_FUNC(NONE, $(entry), { return false; });
  $endfor;
  
  $for(entry in getEnumEntries("BoundingObjectType")[:-1]);
  COLLIDE_FUNC(ALL, $(entry), { return true; });
  $endfor;

  $for(entry in getEnumEntries("BoundingObjectType")[:-1]);
  CONTAINED_IN_FUNC(ALL, $(entry), { return false; });
  $endfor;
}
