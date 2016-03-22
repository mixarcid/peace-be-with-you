#include "BoundingGround.hpp"
#include "BoundingSphere.hpp"
#include "BoundingOBB.hpp"
#include "Manifold.hpp"

NAMESPACE {

  BoundingGround::BoundingGround(GroundDataFunc func)
    : dataAtPoint(func) {}
  
  f32 BoundingGround::getVolume() {
    PEACE_UNIMPLIMENTED(0);
  }
  
  f32 BoundingGround::getInertia(f32 mass) {
    PEACE_UNIMPLIMENTED(0);
  }

  //Isn't perfect at all, but may work
  COLLIDE_FUNC(SPHERE, GROUND, {

      BoundingSphere* a = (BoundingSphere*) oa;
      BoundingGround* b = (BoundingGround*) ob;

      f32 lowest_z = a->center.z() - a->radius;
      return (lowest_z < b->dataAtPoint(a->center.xy(), NULL)); 
    });
  
  MANIFOLD_FUNC(OBB, GROUND, {

      BoundingOBB* a = (BoundingOBB*) oa;
      BoundingGround* b = (BoundingGround*) ob;

      for (i8 axis = 2; axis >= 0; --axis) {

	Vec3f proj;
	switch(axis) {
	case 0:
	  proj = a->coord*Vec3f(a->halves.x(),0,0);
	  break;
	case 1:
	  proj = a->coord*Vec3f(0,a->halves.y(),0);
	  break;
	case 2:
	  proj = a->coord*Vec3f(0,0,a->halves.z());
	  break;
	}

	for (u8 s = 0; s < 2; ++s) {
	  Vec3f point = a->center - (s ? proj : -proj);
	  //Log::message(to_string(point));
	  man->penetration = b->dataAtPoint
	    (point.xy(), &man->normal) - point.z();
	  if (man->penetration > 0.0f) {
	    man->normal = -man->normal;
	    man->penetration *= abs(man->normal.z());
	    return true;
	  }
	}
      }

      return false;
    
    });
}
