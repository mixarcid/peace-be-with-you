#pragma once

#include "BoundingObject.hpp"

NAMESPACE {

  struct Manifold {

    Vec3f normal;
    f32 penetration;
    u32 num_contacts;
    Vec3f contacts[8];
    
  };

  /*//Here we briefly depart my usual nomenclature for the sake of symmetry
  bool SphereVsSphere(BoundingSphere a, BoundingSphere b);
  bool SphereVsSphere(BoundingSphere a, BoundingSphere b, Manifold* man);

  bool SphereVsOBB(BoundingSphere a, BoundingOBB b);
  bool SphereVsOBB(BoundingSphere a, BoundingOBB b, Manifold* man);

  bool OBBVsSphere(BoundingOBB a, BoundingSphere b);
  bool OBBVsSphere(BoundingOBB a, BoundingSphere b, Manifold* man);

  bool OBBVsOBB(BoundingOBB a, BoundingOBB b);
  bool OBBVsOBB(BoundingOBB a, BoundingOBB b, Manifold* man);*/

  bool testIntersection(BoundingObject a,
			BoundingObject b);
  bool testIntersection(BoundingObject a,
			BoundingObject b,
			Manifold* man);

}