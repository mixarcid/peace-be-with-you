#include "BoundingSphere.hpp"
#include "BoundingAABB.hpp"
#include "BoundingOBB.hpp"
#include "Manifold.hpp"

NAMESPACE {

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

  void BoundingSphere::transform(Transform t) {
    center += t.trans;
  }

  //thanks, nerdinand from StackOverFlow
  COLLIDE_FUNC(SPHERE, AABB, {

      BoundingSphere* sphere = (BoundingSphere*) oa;
      BoundingAABB* aabb = (BoundingAABB*) ob;
    
      Vec3f box_min = aabb->center - aabb->halves;
      Vec3f box_max = aabb->center + aabb->halves;
      f32 min_dist = 0;

      if (sphere->center.x() < box_min.x()) {
	min_dist += sqr(sphere->center.x() - box_min.x());
      } else if (sphere->center.x() > box_max.x()) {
	min_dist += sqr(sphere->center.x() + box_max.x());
      }

      if (sphere->center.y() < box_min.y()) {
	min_dist += sqr(sphere->center.y() - box_min.y());
      } else if (sphere->center.y() > box_max.y()) {
	min_dist += sqr(sphere->center.y() + box_max.y());
      }

      if (sphere->center.z() < box_min.z()) {
	min_dist += sqr(sphere->center.z() - box_min.z());
      } else if (sphere->center.z() > box_max.z()) {
	min_dist += sqr(sphere->center.z() + box_max.z());
      }

      return min_dist < sqr(sphere->radius);
    });

  COLLIDE_FUNC(SPHERE, SPHERE, {
      BoundingSphere* a = (BoundingSphere*) oa;
      BoundingSphere* b = (BoundingSphere*) ob;
    
      Vec3f d = a->center - b->center;
      f32 dsq = Vec3f::dot(d,d);
      f32 rsq = sqr(a->radius + b->radius);
      return dsq < rsq;
      
    });
  
  MANIFOLD_FUNC(SPHERE, SPHERE, {

      BoundingSphere* a = (BoundingSphere*) oa;
      BoundingSphere* b = (BoundingSphere*) ob;
    
      Vec3f d = a->center - b->center;
      f32 dsq = Vec3f::dot(d,d);
      f32 r = a->radius + b->radius;
      f32 rsq = sqr(r);
    
      if (dsq < rsq) {
	f32 dist = sqrt(dsq);
	if (dist != 0) {
	  man->penetration = r - dist;
	  man->normal = d/dist;
	} else {
	  man->penetration = a->radius;
	  man->normal = d/dist;
	}
	return true;
      } else {
	return false;
      }
    
    });

  COLLIDE_FUNC(SPHERE, OBB, {

      BoundingSphere* a = (BoundingSphere*) oa;
      BoundingOBB* b = (BoundingOBB*) ob;
    
      Vec3f d = b->getClosestPoint(a->center) - a->center;
      return Vec3f::dot(d,d) > sqr(a->radius);
    
    });

}
