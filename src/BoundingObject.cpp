#include "BoundingObject.hpp"
#include "Node.hpp"

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

  void BoundingSphere::transform(Node* t) {
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

    //just computes the AABB, currently
    debugAssert(data.size() > 0,
		"Why does your mesh have no data?");
    Vec3f max, min = data[0].pos;

    for (BasicMeshData d : data) {
      max.x = max.x > d.pos.x ? max.x : d.pos.x;
      max.y = max.y > d.pos.y ? max.y : d.pos.y;
      max.z = max.z > d.pos.z ? max.z : d.pos.z;
      min.x = min.x < d.pos.x ? min.x : d.pos.x;
      min.y = min.y < d.pos.y ? min.y : d.pos.y;
      min.z = min.z < d.pos.z ? min.z : d.pos.z;
    }

    center = (max + min)/2;
    halves = max - center;
    coord[0] = Vec3f(1,0,0);
    coord[1] = Vec3f(0,1,0);
    coord[2] = Vec3f(0,0,1);

  }

  f32 BoundingOBB::getVolume() {
    return halves.x*halves.y*halves.z*8;
  }

  f32 BoundingOBB::getInertia(f32 mass) {
    //let's pretend it's a cube
    f32 size = (halves.x+halves.y+halves.z)*(2/3);
    return (mass*sqr(size))/6;
  }

  void BoundingOBB::transform(Node* t) {
    center += t->trans;
    coord[0] = t->rot * coord[0];
    coord[1] = t->rot * coord[1];
    coord[2] = t->rot * coord[2];
  }

  Vec3f BoundingOBB::getClosestPoint(Vec3f point) {
    
    Vec3f d = point - center;
    Vec3f ret = center;

    for (u8 i = 0; i < 3; ++i) {
      f32 dist = Vec3f::dot(d, coord[i]);
      if (dist > halves[i]) {
	dist = halves[i];
      }
      if (dist < -halves[i]) {
	dist = -halves[i];
      }
      ret += coord[i]*dist;
    }
    return ret;
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
      rb = b.halves[0] * ar(i,0) +
	b.halves[1] * ar(i,1) + b.halves[2] * ar(i,2);
      if (abs(t[i]) > ra + rb) return false;
    }

    for (u8 i = 0; i < 3; ++i) {
      ra = halves[0] * ar(0,i) +
	halves[1] * ar(1,i) + halves[2] * ar(2,i);
      rb = b.halves[i];
      if (abs(t[0] * r(0,i) + t
	      [1] * r(1,i) + t[2] * r(2,i)) > ra + rb) return false;
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

  bool BoundingOBB::testIntersection(BoundingSphere b) {
    Vec3f d = getClosestPoint(b.center) - b.center;
    return Vec3f::dot(d,d) > sqr(b.radius);
  }

  BoundingObject::BoundingObject(BoundingObjectType obj_type,
				 Array<BasicMeshData> data)
    : type(obj_type) {
    switch(type) {
    case BOUNDING_SPHERE:
      sphere = BoundingSphere(data);
    case BOUNDING_OBB:
      obb = BoundingOBB(data);
    case BOUNDING_NONE:
      return;
    }
  }

  f32 BoundingObject::getVolume() {
    switch(type) {
    case BOUNDING_SPHERE:
      return sphere.getVolume();
    case BOUNDING_OBB:
      return obb.getVolume();
    case BOUNDING_NONE:
      return 0;
    }
  }

  f32 BoundingObject::getInertia(f32 mass) {
    switch(type) {
    case BOUNDING_SPHERE:
      return sphere.getInertia(mass);
    case BOUNDING_OBB:
      return obb.getInertia(mass);
    case BOUNDING_NONE:
      return 0;
    }
  }

  void BoundingObject::transform(Node* t) {
    switch (type) {
    case BOUNDING_SPHERE:
      sphere.transform(t);
    case BOUNDING_OBB:
      obb.transform(t);
    case BOUNDING_NONE:
      ;
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

  Manifold::Manifold(BoundingOBB a, BoundingOBB b) {
    
  }

  Manifold::Manifold(BoundingSphere a, BoundingOBB b) {
    
  }

  Manifold::Manifold(BoundingOBB a, BoundingSphere b) {
    
  }
  
  Manifold::Manifold(BoundingObject a, BoundingObject b) {
    switch (a.type) {
    case BOUNDING_SPHERE:
      switch(b.type) {
      case BOUNDING_SPHERE:
	*this = Manifold(a.sphere, b.sphere);
        return;
      case BOUNDING_OBB:
	*this = Manifold(a.sphere, b.obb);
	return;
      case BOUNDING_NONE:
	return;
      }
    case BOUNDING_OBB:
      switch(b.type) {
      case BOUNDING_SPHERE:
	*this = Manifold(a.obb, b.sphere);
	return;
      case BOUNDING_OBB:
	*this = Manifold(a.obb, b.obb);
	return;
      case BOUNDING_NONE:
	return;
      }
    case BOUNDING_NONE:
      return;
    }
  }

  bool testIntersection(BoundingObject a, BoundingObject b) {
    switch (a.type) {
    case BOUNDING_SPHERE:
      switch(b.type) {
      case BOUNDING_SPHERE:
	return a.sphere.testIntersection(b.sphere);
      case BOUNDING_OBB:
	return b.obb.testIntersection(a.sphere);
      case BOUNDING_NONE:
	return false;
      }
    case BOUNDING_OBB:
      switch(b.type) {
      case BOUNDING_SPHERE:
	return a.obb.testIntersection(b.sphere);
      case BOUNDING_OBB:
	return a.obb.testIntersection(b.obb);
      case BOUNDING_NONE:
	return false;
      }
    case BOUNDING_NONE:
      return false;
    }
  }
    
}
