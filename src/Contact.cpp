#include "Contact.hpp"

NAMESPACE {

  bool SphereVsSphere(BoundingSphere a, BoundingSphere b) {
    //Log::message("testing...");
    //Log::message(a.center.toString() + " " + b.center.toString());
    Vec3f d = a.center - b.center;
    f32 dsq = Vec3f::dot(d,d);
    f32 rsq = sqr(a.radius + b.radius);
    //Log::message("d: " + d.toString());
    //Log::message("rsq: " + to_string(rsq) + "\n");
    return dsq < rsq;
  }
  
  bool SphereVsSphere(BoundingSphere a, BoundingSphere b, Manifold* man) {
    Vec3f d = a.center - b.center;
    f32 dsq = Vec3f::dot(d,d);
    f32 r = a.radius + b.radius;
    f32 rsq = sqr(r);
    
    if (dsq < rsq) {
      f32 dist = sqrt(dsq);
      if (dist != 0) {
	man->penetration = r - dist;
	man->normal = d/dist;
      } else {
	man->penetration = a.radius;
	man->normal = d/dist;
      }
      return true;
    } else {
      return false;
    }
  }

  bool SphereVsOBB(BoundingSphere a, BoundingOBB b) {
    Vec3f d = b.getClosestPoint(a.center) - a.center;
    return Vec3f::dot(d,d) > sqr(a.radius);
  }
  
  bool SphereVsOBB(BoundingSphere a, BoundingOBB b, Manifold* man) {
    return false;
  }

  bool OBBVsSphere(BoundingOBB a, BoundingSphere b) {
    return SphereVsOBB(b, a);
  }
  
  bool OBBVsSphere(BoundingOBB a, BoundingSphere b, Manifold* man) {
    return false;
  }

  const f32 OBB_TEST_EPSILON = 0.0001;
  
  bool OBBVsOBB(BoundingOBB a, BoundingOBB b) {

    f32 ra, rb;
    Mat3f r, ar;

    r = a.coord*b.coord;
    
    Vec3f t = b.center - a.center;
    t = a.coord.vecMul(t);
    //Log::message("");
    
    for (u8 i = 0; i < 3; ++i) {
      for (u8 j = 0; j < 3; ++j) {
	ar.data[3*i + j] = abs(r(i,j)) + OBB_TEST_EPSILON;
      }
    }

    //Log::message(ar.toString());
    //Now let's test some axes!

    //Log::message("T: " + t.toString());
    //Log::message("%f", t[8]);
    //a's axes
    for (u8 i = 0; i < 3; ++i) {
      ra = a.halves[i];
      rb = Vec3f::dot(b.halves, ar[i]);
      //Log::message(to_string((t[i])));
      //Log::message("%f", ra + rb);
      if (abs(t[i]) > ra + rb) return false;
    }

    //b's axes
    for (u8 i = 0; i < 3; ++i) {
      ra = Vec3f::dot(a.halves, ar[i]);
      rb = b.halves[i];
      if (abs(t[0] * r(0,i) + t
	      [1] * r(1,i) + t[2] * r(2,i)) > ra + rb) return false;
    }

    ra = a.halves[1] * ar(2,0) + a.halves[2] * ar(1,0);
    rb = b.halves[1] * ar(0,2) + b.halves[2] * ar(0,1);
    if (abs(t[2] * r(1,0) - t[1] * r(2,0)) > ra + rb) return false;

    ra = a.halves[1] * ar(2,1) + a.halves[2] * ar(1,1);
    rb = b.halves[0] * ar(0,2) + b.halves[2] * ar(0,0);
    if (abs(t[2] * r(1,1) - t[1] * r(2,1)) > ra + rb) return false;

    ra = a.halves[1] * ar(2,2) + a.halves[2] * ar(1,2);
    rb = b.halves[0] * ar(0,1) + b.halves[1] * ar(0,0);
    if (abs(t[2] * r(1,2) - t[1] * r(2,2)) > ra + rb) return false;

    ra = a.halves[0] * ar(2,0) + a.halves[2] * ar(0,0);
    rb = b.halves[1] * ar(1,2) + b.halves[2] * ar(1,1);
    if (abs(t[0] * r(2,0) - t[2] * r(0,0)) > ra + rb) return false;

    ra = a.halves[0] * ar(2,1) + a.halves[2] * ar(0,1);
    rb = b.halves[0] * ar(1,2) + b.halves[2] * ar(1,0);
    if (abs(t[0] * r(2,1) - t[2] * r(0,1)) > ra + rb) return false;

    ra = a.halves[0] * ar(2,2) + a.halves[2] * ar(0,2);
    rb = b.halves[0] * ar(1,1) + b.halves[1] * ar(1,0);
    if (abs(t[0] * r(2,2) - t[2] * r(0,2)) > ra + rb) return false;

    ra = a.halves[0] * ar(1,0) + a.halves[1] * ar(0,0);
    rb = b.halves[1] * ar(2,2) + b.halves[2] * ar(2,1);
    if (abs(t[1] * r(0,0) - t[0] * r(1,0)) > ra + rb) return false;

    ra = a.halves[0] * ar(1,1) + a.halves[1] * ar(0,1);
    rb = b.halves[0] * ar(2,2) + b.halves[2] * ar(2,0);
    if (abs(t[1] * r(0,1) - t[0] * r(1,1)) > ra + rb) return false;

    ra = a.halves[0] * ar(1,2) + a.halves[1] * ar(0,2);
    rb = b.halves[0] * ar(2,1) + b.halves[1] * ar(2,0);
    if (abs(t[1] * r(0,2) - t[0] * r(1,2)) > ra + rb) return false;
 
    return true;
  }
  
  bool OBBVsOBB(BoundingOBB a, BoundingOBB b, Manifold* man) {
    return OBBVsOBB(a,b);
  }

  bool testIntersection(BoundingObject a, BoundingObject b) {
    switch (a.type) {
    case BOUNDING_SPHERE:
      switch(b.type) {
      case BOUNDING_SPHERE:
	return SphereVsSphere(a.sphere, b.sphere);
      case BOUNDING_OBB:
	return SphereVsOBB(a.sphere, b.obb);
      case BOUNDING_NONE:
	return false;
      }
    case BOUNDING_OBB:
      switch(b.type) {
      case BOUNDING_SPHERE:
	return OBBVsSphere(a.obb, b.sphere);
      case BOUNDING_OBB:
	return OBBVsOBB(a.obb, b.obb);
      case BOUNDING_NONE:
	return false;
      }
    case BOUNDING_NONE:
      return false;
    }
    return false;
  }

  bool testIntersection(BoundingObject a,
			BoundingObject b,
			Manifold* man) {
    switch (a.type) {
    case BOUNDING_SPHERE:
      switch(b.type) {
      case BOUNDING_SPHERE:
	return SphereVsSphere(a.sphere, b.sphere, man);
      case BOUNDING_OBB:
	return SphereVsOBB(a.sphere, b.obb, man);
      case BOUNDING_NONE:
	return false;
      }
    case BOUNDING_OBB:
      switch(b.type) {
      case BOUNDING_SPHERE:
	return OBBVsSphere(a.obb, b.sphere, man);
      case BOUNDING_OBB:
	return OBBVsOBB(a.obb, b.obb, man);
      case BOUNDING_NONE:
	return false;
      }
    case BOUNDING_NONE:
      return false;
    }
    return false;
  }
  
}
