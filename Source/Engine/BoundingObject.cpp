#include "BoundingObject.hpp"

NAMESPACE {

  BoundingAABB::BoundingAABB(Vec3f _center, Vec3f _halves)
    : center(_center), halves(_halves) {}

  f32 BoundingAABB::getVolume() {
    return halves.x()*halves.y()*halves.z()*8;
  }
  
  f32 BoundingAABB::getInertia(f32 mass) {
    f32 size = (halves.x()+halves.y()+halves.z())*(2/3);
    return (mass*sqr(size))/6;
  }
  
  bool BoundingAABB::someInBox(BoundingAABB box) {
    
    if (abs(center.x() - box.center.x())
	> (halves.x() + box.halves.x())) return false;
    if (abs(center.y() - box.center.y())
	> (halves.y() + box.halves.y())) return false;
    if (abs(center.z() - box.center.z())
	> (halves.z() + box.halves.z())) return false;

    return true;
  }
  
  bool BoundingAABB::allInBox(BoundingAABB box) {
    
    return ((abs(center.x() - box.center.x())
	     < (halves.x() + box.halves.x())) &&
	    (abs(center.y() - box.center.y())
	     < (halves.y() + box.halves.y())) &&
	    (abs(center.z() - box.center.z())
	     < (halves.z() + box.halves.z())));
  }
  
  void BoundingAABB::transform(Transform t) {
    PEACE_UNIMPLIMENTED();
  }

  BoundingSphere::BoundingSphere(Array<BasicMeshData>& data) {
    
    //Ritter's algorithm
    debugAssert(data.size() > 0,
		"Why does your mesh have no data?");
    Vec3f x = data[0].pos;
    Vec3f y;
    Vec3f z;
    f32 dist = 0;
    f32 test_dist = 0;

    for (BasicMeshData d : data) {
      test_dist = (d.pos -x).norm();
      if (test_dist > dist) {
	dist = test_dist;
	y = d.pos;
      }
    }
    
    dist = 0;
    test_dist = 0;
    for (BasicMeshData d : data) {
      test_dist = (d.pos - y).norm();
      if (test_dist > dist) {
	dist = test_dist;
	z = d.pos;
      }
    }

    radius = dist/2;
    center = (z + y)/2;

    f32 r_test = radius;
    for (BasicMeshData d : data) {
      r_test = (d.pos - center).norm();
      radius = r_test > radius ? r_test : radius;
    }

  }

  f32 BoundingSphere::getVolume() {
    return (4*M_PI/3)*radius*radius*radius;
  }

  f32 BoundingSphere::getInertia(f32 mass) {
    return (2/5)*mass*sqr(radius);
  }

  //thanks, nerdinand from StackOverFlow
  bool BoundingSphere::someInBox(BoundingAABB box) {
    
    Vec3f box_min = box.center - box.halves;
    Vec3f box_max = box.center + box.halves;
    f32 min_dist = 0;

    if (this->center.x() < box_min.x()) {
      min_dist += sqr(this->center.x() - box_min.x());
    } else if (this->center.x() > box_max.x()) {
      min_dist += sqr(this->center.x() + box_max.x());
    }

    if (this->center.y() < box_min.y()) {
      min_dist += sqr(this->center.y() - box_min.y());
    } else if (this->center.y() > box_max.y()) {
      min_dist += sqr(this->center.y() + box_max.y());
    }

    if (this->center.z() < box_min.z()) {
      min_dist += sqr(this->center.z() - box_min.z());
    } else if (this->center.z() > box_max.z()) {
      min_dist += sqr(this->center.z() + box_max.z());
    }

    return min_dist < sqr(radius);
  }

  bool BoundingSphere::allInBox(BoundingAABB box) {
    PEACE_UNIMPLIMENTED(false);
  }

  void BoundingSphere::transform(Transform t) {
    center += t.trans;
  }

  BoundingOBB::BoundingOBB(Array<BasicMeshData>& data) {
    
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

  void BoundingOBB::transform(Transform t) {
    center += t.trans;
    coord *= t.rot.mat3();
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

  bool BoundingOBB::someInBox(BoundingAABB box) {
    PEACE_UNIMPLIMENTED(false);
  }

  bool BoundingOBB::allInBox(BoundingAABB box) {

    for (u8 x=0; x<2; ++x) {
      for (u8 y=0; x<2; ++x) {
	for (u8 z=0; x<2; ++x) {
	  
	  Vec3f unoriented_vert(x ? this->halves.x() : -this->halves.x(),
				y ? this->halves.y() : -this->halves.y(),
				z ? this->halves.z() : -this->halves.z());
	  Vec3f vert = this->center + (coord*unoriented_vert);
	  Vec3f rel_center = box.center - vert;
	  
	  if (abs(rel_center.x()) > box.halves.x() ||
	      abs(rel_center.y()) > box.halves.y() ||
	      abs(rel_center.z()) > box.halves.z()) return false;
	  
	}
      }
    }
    return true;
  }

  f32 BoundingGround::getVolume() {
    PEACE_UNIMPLIMENTED(0);
  }
  
  f32 BoundingGround::getInertia(f32 mass) {
    PEACE_UNIMPLIMENTED(0);
  }
  
  bool BoundingGround::someInBox(BoundingAABB box) {
    return true;
  }
  
  bool BoundingGround::allInBox(BoundingAABB box) {
    return false;

  }

  void BoundingGround::transform(Transform t) {
    PEACE_UNIMPLIMENTED();
  }

  BoundingObject::BoundingObject(BoundingObjectType _type) : type(_type) {}

  BoundingObject::BoundingObject(BoundingObjectType obj_type,
				 Array<BasicMeshData>& data)
    : type(obj_type) {
    switch(type) {
    case BOUNDING_SPHERE:
      sphere = BoundingSphere(data);
      break;
    case BOUNDING_OBB:
      obb = BoundingOBB(data);
      break;
    case BOUNDING_GROUND:
      Log::fatalError("Cannot call the BoundingObject constuctor with"
		      " type BOUNDING_GROUND");
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
    case BOUNDING_GROUND:
      return ground.getVolume();
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
    case BOUNDING_GROUND:
      return ground.getInertia(mass);
    case BOUNDING_NONE:
      return 0;
    }
    return 0;
  }
  
  bool BoundingObject::someInBox(BoundingAABB box) {
    switch(type) {
    case BOUNDING_SPHERE:
      return sphere.someInBox(box);
    case BOUNDING_OBB:
      return obb.someInBox(box);
    case BOUNDING_GROUND:
      return ground.someInBox(box);
    case BOUNDING_NONE:
      return false;
    }
    return false;
  }
  
  bool BoundingObject::allInBox(BoundingAABB box) {
    switch(type) {
    case BOUNDING_SPHERE:
      return sphere.allInBox(box);
    case BOUNDING_OBB:
      return obb.allInBox(box);
    case BOUNDING_GROUND:
      return ground.allInBox(box);
    case BOUNDING_NONE:
      return false;
    }
    return false;
  }

  void BoundingObject::transform(Transform t) {
    switch (type) {
    case BOUNDING_SPHERE:
      sphere.transform(t);
      break;
    case BOUNDING_OBB:
      obb.transform(t);
      break;
    case BOUNDING_GROUND:
      ground.transform(t);
      break;
    case BOUNDING_NONE:
      ;
    }
  }
  
  BoundingObject& BoundingObject::operator=(const BoundingObject& b) {
    memcpy(this,&b,sizeof(BoundingObject));
    return *this;
  }
  
}
