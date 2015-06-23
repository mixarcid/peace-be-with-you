#include "BoundingObject.hpp"
#include "Transform.hpp"

NAMESPACE {

  BoundingSphere::BoundingSphere(Array<BasicMeshData> data) {
    
    //Ritter's algorithm
    debugAssert(data.size() > 0,
		"Why does your mesh have no data?");
    Vec3f x = data[0].pos;
    Vec3f y;
    Vec3f z;
    f32 dist = 0;
    f32 test_dist = 0;

    for (BasicMeshData d : data) {
      //Log::message(d.pos.toString());
      test_dist = (d.pos - x).abs();
      if (test_dist > dist) {
	dist = test_dist;
	y = d.pos;
      }
    }
    //Log::message("D: %f", dist);
    //Log::message("X: " + x.toString());
    //Log::message("Y: " + y.toString());

    dist = 0;
    test_dist = 0;
    for (BasicMeshData d : data) {
      test_dist = (d.pos - y).abs();
      if (test_dist > dist) {
	dist = test_dist;
	z = d.pos;
      }
    }
    //Log::message("Z: " + z.toString());

    radius = dist/2;
    center = (z + y)/2;

    f32 r_test = radius;
    for (BasicMeshData d : data) {
      r_test = (d.pos - center).abs();
      radius = r_test > radius ? r_test : radius;
    }

    //Log::message("center: " + center.toString()
    // + "\nRadius: " + to_string(radius));
  }

  f32 BoundingSphere::getVolume() {
    return (4*M_PI/3)*radius*radius*radius;
  }

  f32 BoundingSphere::getInertia(f32 mass) {
    return (2/5)*mass*sqr(radius);
  }

  void BoundingSphere::transform(Transform* t) {
    center += t->trans;
  }

  bool BoundingSphere::testIntersection(BoundingSphere b) {
    //Log::message("testing...");
    Vec3f d = center - b.center;
    f32 dsq = Vec3f::dot(d,d);
    f32 rsq = sqr(radius + b.radius);
    //Log::message("d: " + d.toString());
    //Log::message("rsq: " + to_string(rsq) + "\n");
    return dsq < rsq;
  }

  BoundingOBB::BoundingOBB(Array<BasicMeshData> data) {

  }

  const f32 OBB_TEST_EPSILON = 0.0001;
  bool BoundingOBB::testIntersection(BoundingOBB b) {
    
    f32 ra, rb;
    Mat3f r, ar;

    for (u8 i = 0; i < 3; ++i) {
      for (u8 j = 0; j < 3; ++j) {
	r.data[i*3 + j] = Vec3f::dot(coord[i], b.coord[j]); 
      }
    }
    
    Vec3f t = b.center - center;
    t = Vec3f(Vec3f::dot(t, coord[0]),
	      Vec3f::dot(t, coord[1]),
	      Vec3f::dot(t, coord[2]));
    
    for (u8 i = 0; i < 3; ++i) {
      for (u8 j = 0; j < 3; ++j) {
	ar.data[3*i + j] = abs(r(i,j)) + OBB_TEST_EPSILON;
      }
    }

    //Now let's test some axes!

    for (u8 i = 0; i < 3; ++i) {
      ra = halves[i];
      rb = b.halves[0] * ar(i,0) + b.halves[1] * ar(i,1) + b.halves[2] * ar(i,2);
      if (abs(t[i]) > ra + rb) return false;
    }

    for (u8 i = 0; i < 3; ++i) {
      ra = halves[0] * ar(0,i) + halves[1] * ar(1,i) + halves[2] * ar(2,i);
      rb = b.halves[i];
      if (abs(t[0] * r(0,i) + t[1] * r(1,i) + t[2] * r(2,i)) > ra + rb) return false;
    }

    ra = halves[1] * ar(2,0) + halves[2] * ar(1,0);
    rb = b.halves[1] * ar(0,2) + b.halves[2] * ar(0,1);
    if (abs(t[2] * r(1,0) - t[1] * r(2,0)) > ra + rb) return false;

    ra = halves[1] * ar(2,1) + halves[2] * ar(1,1);
    rb = b.halves[0] * ar(0,2) + b.halves[2] * ar(0,0);
    if (abs(t[2] * r(1,1) - t[1] * r(2,1)) > ra + rb) return false;

    ra = halves[1] * ar(2,2) + halves[2] * ar(1,2);
    rb = b.halves[0] * ar(0,1) + b.halves[1] * ar(0,0);
    if (abs(t[2] * r(1,2) - t[1] * r(2,2)) > ra + rb) return false;

    ra = halves[0] * ar(2,0) + halves[2] * ar(0,0);
    rb = b.halves[1] * ar(1,2) + b.halves[2] * ar(1,1);
    if (abs(t[0] * r(2,0) - t[2] * r(0,0)) > ra + rb) return false;

    ra = halves[0] * ar(2,1) + halves[2] * ar(0,1);
    rb = b.halves[0] * ar(1,2) + b.halves[2] * ar(1,0);
    if (abs(t[0] * r(2,1) - t[2] * r(0,1)) > ra + rb) return false;

    ra = halves[0] * ar(2,2) + halves[2] * ar(0,2);
    rb = b.halves[0] * ar(1,1) + b.halves[1] * ar(1,0);
    if (abs(t[0] * r(2,2) - t[2] * r(0,2)) > ra + rb) return false;

    ra = halves[0] * ar(1,0) + halves[1] * ar(0,0);
    rb = b.halves[1] * ar(2,2) + b.halves[2] * ar(2,1);
    if (abs(t[1] * r(0,0) - t[0] * r(1,0)) > ra + rb) return false;

    ra = halves[0] * ar(1,1) + halves[1] * ar(0,1);
    rb = b.halves[0] * ar(2,2) + b.halves[2] * ar(2,0);
    if (abs(t[1] * r(0,1) - t[0] * r(1,1)) > ra + rb) return false;

    ra = halves[0] * ar(1,2) + halves[1] * ar(0,2);
    rb = b.halves[0] * ar(2,1) + b.halves[1] * ar(2,0);
    if (abs(t[1] * r(0,2) - t[0] * r(1,2)) > ra + rb) return false;
 
    return true;
  }

  BoundingObject::BoundingObject(BoundingObjectType obj_type,
				 Array<BasicMeshData> data)
    : type(obj_type) {
    switch(type) {
    case BOUNDING_SPHERE:
      sphere = BoundingSphere(data);
    }
  }

  f32 BoundingObject::getVolume() {
    switch(type) {
    case BOUNDING_SPHERE:
      return sphere.getVolume();
    }
  }

  f32 BoundingObject::getInertia(f32 mass) {
    switch(type) {
    case BOUNDING_SPHERE:
      return sphere.getInertia(mass);
    }
  }

  void BoundingObject::transform(Transform* t) {
    switch (type) {
    case BOUNDING_SPHERE:
      sphere.transform(t);
    }
  }

  Manifold::Manifold(BoundingSphere a, BoundingSphere b) {
    Vec3f n = (a.center - b.center);
    f32 d = n.abs();
    f32 r = a.radius + b.radius;
    if (d != 0) {
      penetration = r - d;
      //Log::message("P: %f", penetration);
      normal = n/d;
      //Log::message("n: " + normal.toString());
    } else {
      penetration = a.radius;
      normal = Vec3f(1,0,0);
    }
  }
  
  Manifold::Manifold(BoundingObject a, BoundingObject b) {
    switch (a.type) {
    case BOUNDING_SPHERE:
      switch(b.type) {
      case BOUNDING_SPHERE:
	*this = Manifold(a.sphere, b.sphere);
      }
    }
  }

  bool testIntersection(BoundingObject a, BoundingObject b) {
    switch (a.type) {
    case BOUNDING_SPHERE:
      switch(b.type) {
      case BOUNDING_SPHERE:
	return a.sphere.testIntersection(b.sphere);
      }
    }
  }
    
}
