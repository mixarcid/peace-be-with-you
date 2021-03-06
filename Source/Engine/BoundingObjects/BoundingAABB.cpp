#include "BoundingAABB.hpp"
#include "RenderableShape.hpp"

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

  void BoundingAABB::transform(Transform t) {
    center += t.trans;
  }

  Pointer<RenderableComp> BoundingAABB::getRenderable(Mat4f* model) {
    *model = (Mat4f::translate(center) *
	      Mat4f::scale(halves));
    return (RenderableComp*) &RenderableShape::CUBE;
  }
  
  Vec3f BoundingAABB::getCenter() {
    return center;
  }
  
  COLLIDE_FUNC(AABB, AABB, {

      BoundingAABB* a = (BoundingAABB*) oa;
      BoundingAABB* b = (BoundingAABB*) ob;
    
      if (abs(a->center.x() - b->center.x())
	  > (a->halves.x() + b->halves.x())) return false;
      if (abs(a->center.y() - b->center.y())
	  > (a->halves.y() + b->halves.y())) return false;
      if (abs(a->center.z() - b->center.z())
	  > (a->halves.z() + b->halves.z())) return false;

      return true;
    });
  
  CONTAINED_IN_FUNC(AABB, AABB, {

      BoundingAABB* a = (BoundingAABB*) oa;
      BoundingAABB* b = (BoundingAABB*) ob;
    
      return ((abs(a->center.x() - b->center.x())
	       < (a->halves.x() + b->halves.x())) &&
	      (abs(a->center.y() - b->center.y())
	       < (a->halves.y() + b->halves.y())) &&
	      (abs(a->center.z() - b->center.z())
	       < (a->halves.z() + b->halves.z())));
    });
  
}
