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
      test_dist = (d.pos -x).norm();
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
      test_dist = (d.pos - y).norm();
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
      r_test = (d.pos - center).norm();
      radius = r_test > radius ? r_test : radius;
    }

    /*Log::message("Center: " + center.toString()
      + "\nRadius: " + to_string(radius));*/
  }

  f32 BoundingSphere::getVolume() {
    return (4*M_PI/3)*radius*radius*radius;
  }

  f32 BoundingSphere::getInertia(f32 mass) {
    return (2/5)*mass*sqr(radius);
  }

  void BoundingSphere::transform(Node* t) {
    //Log::message(center.toString());
    center += t->trans;
  }

  BoundingOBB::BoundingOBB(Array<BasicMeshData> data) {
    
    //just computes the AABB, currently
    debugAssert(data.size() > 0,
		"Why does your mesh have no data?");
    Vec3f max, min = data[0].pos;

    for (BasicMeshData d : data) {
      max.data[0] = max.x() > d.pos.x() ? max.x() : d.pos.x();
      max.data[1] = max.y() > d.pos.y() ? max.y() : d.pos.y();
      max.data[2] = max.z() > d.pos.z() ? max.z() : d.pos.z();
      min.data[0] = min.x() < d.pos.x() ? min.x() : d.pos.x();
      min.data[1] = min.y() < d.pos.y() ? min.y() : d.pos.y();
      min.data[2] = min.z() < d.pos.z() ? min.z() : d.pos.z();
    }

    center = (max + min)/2;
    halves = max - center;
  }

  f32 BoundingOBB::getVolume() {
    return halves.x()*halves.y()*halves.z()*8;
  }

  f32 BoundingOBB::getInertia(f32 mass) {
    //let's pretend it's a cube
    f32 size = (halves.x()+halves.y()+halves.z())*(2/3);
    return (mass*sqr(size))/6;
  }

  void BoundingOBB::transform(Node* t) {
    center += t->trans;
    coord *= t->rot.mat3();
  }

  Vec3f BoundingOBB::getClosestPoint(Vec3f point) {
    
    Vec3f d = point - center;
    Vec3f ret = center;

    for (u8 i = 0; i < 3; ++i) {
      f32 dist = Vec3f::dot(d, coord.row(i));
      if (dist > halves[i]) {
	dist = halves[i];
      }
      if (dist < -halves[i]) {
	dist = -halves[i];
      }
      ret += coord.row(i)*dist;
    }
    return ret;
  }

  BoundingObject::BoundingObject() : type(BOUNDING_NONE) {}

  BoundingObject::BoundingObject(BoundingObjectType obj_type,
				 Array<BasicMeshData> data)
    : type(obj_type) {
    switch(type) {
    case BOUNDING_SPHERE:
      sphere = BoundingSphere(data);
      break;
    case BOUNDING_OBB:
      obb = BoundingOBB(data);
      break;
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
    return 0;
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
    return 0;
  }

  void BoundingObject::transform(Node* t) {
    switch (type) {
    case BOUNDING_SPHERE:
      sphere.transform(t);
      break;
    case BOUNDING_OBB:
      obb.transform(t);
      break;
    case BOUNDING_NONE:
      ;
    }
  }
}
