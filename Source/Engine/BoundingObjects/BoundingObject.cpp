#include "BoundingObject.hpp"
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
    BoundingObject::collide_functions[b][a] = func;
  }

  DefineManifoldFunc::DefineManifoldFunc(BoundingObject::Type a,
					 BoundingObject::Type b,
					 ManifoldFunc func) {
    /*for (u8 x = BoundingObject::AABB; x < BoundingObject::LAST; ++x) {
      for (u8 y = BoundingObject::AABB; y < BoundingObject::LAST; ++y) {
	printf("%u, %u: ", x, y);
	if (BoundingObject::manifold_functions[x][y]) {
	  printf("Yes!\n");
	} else {
	  printf("No :(\n");
	}
      }
    }
    printf("\n");*/
    BoundingObject::manifold_functions[a][b] = func;
    //BoundingObject::manifold_functions[b][a] = func;
      /*[a,b](BoundingObject* oa, BoundingObject* ob, Manifold* m)->bool {
      if (!BoundingObject::manifold_functions[a][b](oa, ob, m)) {
	return false;
      } else {
	m->normal = - m->normal;
	return true;
      }
      };*/
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

  f32 BoundingObject::getVolume() {
    return 0;
  }

  f32 BoundingObject::getInertia(f32 mass) {
    return 0;
  }
  
  BoundingObject* BoundingObject::transform(TransformBasic t) {
    return new BoundingObject(*this);
  }

  $for(entry in getEnumEntries("BoundingObjectType")[:-1]);
  COLLIDE_FUNC(NONE, $(entry), { return true; });
  $endfor;
}
