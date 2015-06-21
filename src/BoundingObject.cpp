#include "BoundingObject.hpp"

NAMESPACE {

  BoundingObject::BoundingObject(BoundingObjectType obj_type)
    : type(obj_type) {}

  BoundingSphere::BoundingSphere(Array<BasicMeshData> data)
    : BoundingObject(BOUNDING_SPHERE) {
    
    //Ritter's algorithm
    Vec3f x = data[0].pos;
    Vec3f y;
    Vec3f z;
    float dist = 0;
    float test_dist = 0;

    for (BasicMeshData d : data) {
      test_dist = (d.pos - x).abs();
      if (test_dist > dist) {
	dist = test_dist;
	y = d.pos;
      }
    }

    dist = 0;
    test_dist = 0;
    for (BasicMeshData d : data) {
      test_dist = (d.pos - y).abs();
      if (test_dist > dist) {
	dist = test_dist;
	y = d.pos;
      }
    }

    radius = dist/2;
    center = (z + y)/2;

    float r_test = radius;
    for (BasicMeshData d : data) {
      r_test = (d.pos - center).abs();
      radius = r_test > radius ? r_test : radius;
    }

    /*Log::message("Center: " + center.toString()
      + "\nRadius: " + to_string(radius));*/
  }

  bool testIntersection(BoundingObject* a, BoundingObject* b) {

    switch (a->type) {
    case BOUNDING_SPHERE:
      switch(b->type) {
      case BOUNDING_SPHERE:
	
	Vec3f d = ((BoundingSphere*) a)->center
	  - ((BoundingSphere*) b)->center;
	float dsq = Vec3f::dot(d,d);
	float rsq = sqr(((BoundingSphere*) a)->radius
			+ ((BoundingSphere*) b)->radius);
	return dsq < rsq;
	  
      }
      break;
    }

  }

}
