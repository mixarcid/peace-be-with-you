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

  //thanks, nerdinand from StackOverFlow
  bool BoundingSphere::someInBox(Vec3f center, Vec3f halves) {
    
    Vec3f box_min = center - halves;
    Vec3f box_max = center + halves;
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

  bool BoundingSphere::allInBox(Vec3f center, Vec3f halves) {
    PEACE_UNIMPLIMENTED(BoundingSphere::allInBox);
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

  bool BoundingOBB::someInBox(Vec3f center, Vec3f halves) {
    PEACE_UNIMPLIMENTED(BoundingOBB::someInBox);
  }

  bool BoundingOBB::allInBox(Vec3f center, Vec3f halves) {

    //Log::message("----");
    //Log::message("center: " + to_string(center));
    for (u8 x=0; x<2; ++x) {
      for (u8 y=0; x<2; ++x) {
	for (u8 z=0; x<2; ++x) {
	  
	  Vec3f unoriented_vert(x ? this->halves.x() : -this->halves.x(),
				y ? this->halves.y() : -this->halves.y(),
				z ? this->halves.z() : -this->halves.z());
	  Vec3f vert = this->center + (coord*unoriented_vert);
	  //Log::message("vert: " + to_string(vert));
	  Vec3f rel_center = center - vert;
	  
	  if (abs(rel_center.x()) > halves.x() ||
	      abs(rel_center.y()) > halves.y() ||
	      abs(rel_center.z()) > halves.z()) return false;
	  
	}
      }
    }
    return true;
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
  
  bool BoundingObject::someInBox(Vec3f center, Vec3f halves) {
    switch(type) {
    case BOUNDING_SPHERE:
      return sphere.someInBox(center, halves);
    case BOUNDING_OBB:
      return obb.someInBox(center, halves);
    case BOUNDING_NONE:
      return false;
    }
    return false;
  }
  
  bool BoundingObject::allInBox(Vec3f center, Vec3f halves) {
    switch(type) {
    case BOUNDING_SPHERE:
      return sphere.allInBox(center, halves);
    case BOUNDING_OBB:
      return obb.allInBox(center, halves);
    case BOUNDING_NONE:
      return false;
    }
    return false;
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
