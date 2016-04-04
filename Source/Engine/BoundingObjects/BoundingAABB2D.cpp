#include "BoundingAABB2D.hpp"
#include "BoundingAABB.hpp"
#include "BoundingSphere.hpp"
#include "BoundingOBB.hpp"

NAMESPACE {

  BoundingAABB2D::BoundingAABB2D(Vec2f _center,
				 Vec2f _halves)
    : center(_center), halves(_halves) {}

  Vec3f BoundingAABB2D::getCenter() {
    return Vec3f(center, 0);
  }

  Vec2f BoundingAABB2D::getClosestPoint(Vec2f point) {
    
    Vec2f d = point - center;
    Vec2f ret = center;

    for (u8 i = 0; i < 2; ++i) {
      f32 dist = d.data[i];
      if (dist > halves[i]) {
	dist = halves[i];
      }
      if (dist < -halves[i]) {
	dist = -halves[i];
      }
      ret.data[i] += dist;
    }
    return ret;
  }

  bool BoundingAABB2D::pointInside(Vec2f point) {
    if (abs(point.x() - center.x()) > halves.x())
      return false;
    if (abs(point.y() - center.y()) > halves.y())
      return false;
    return true;
  }

  
  COLLIDE_FUNC(AABB2D, AABB, {

      BoundingAABB2D* a = (BoundingAABB2D*) oa;
      BoundingAABB* b = (BoundingAABB*) ob;
    
      if (abs(a->center.x() - b->center.x())
	  > (a->halves.x() + b->halves.x())) return false;
      if (abs(a->center.y() - b->center.y())
	  > (a->halves.y() + b->halves.y())) return false;

      return true;
    });

  CONTAINED_IN_FUNC(AABB2D, AABB2D, {
      
      BoundingAABB2D* a = (BoundingAABB2D*) oa;
      BoundingAABB2D* b = (BoundingAABB2D*) ob;
      BoundingAABB2D test(b->center, b->halves - a->halves);
      return test.pointInside(a->center);
    });

  CONTAINED_IN_FUNC(AABB, AABB2D, {
      
      BoundingAABB* a = (BoundingAABB*) oa;
      BoundingAABB2D* b = (BoundingAABB2D*) ob;

      BoundingAABB2D test(b->center, b->halves - a->halves.xy());
      return test.pointInside(a->center.xy());
    });

  COLLIDE_FUNC(AABB2D, AABB2D, {

      BoundingAABB2D* a = (BoundingAABB2D*) oa;
      BoundingAABB2D* b = (BoundingAABB2D*) ob;
    
      if (abs(a->center.x() - b->center.x())
	  > (a->halves.x() + b->halves.x())) return false;
      if (abs(a->center.y() - b->center.y())
	  > (a->halves.y() + b->halves.y())) return false;

      return true;
    });

  COLLIDE_FUNC(AABB2D, SPHERE, {

      BoundingAABB2D* a = (BoundingAABB2D*) oa;
      BoundingSphere* b = (BoundingSphere*) ob;

      Vec2f point = a->getClosestPoint(b->center.xy());
      Vec2f rel = point - b->center.xy();
      
      return Vec2f::dot(rel, rel) < sqr(b->radius);
    });

  CONTAINED_IN_FUNC(SPHERE, AABB2D, {

      BoundingSphere* a = (BoundingSphere*) oa;
      BoundingAABB2D* b = (BoundingAABB2D*) ob;

      BoundingAABB2D test
	(b->center,
	 b->halves - Vec2f(a->radius, a->radius));
      return test.pointInside(a->center.xy());
    });
  
}
