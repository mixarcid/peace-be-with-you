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

  BoundingObject* BoundingGround::transform(TransformBasic t) {
    return new BoundingGround(*this);
  }

  //Isn't perfect at all, but may work
  COLLIDE_FUNC(SPHERE, GROUND, {

      BoundingSphere* a = (BoundingSphere*) oa;
      BoundingGround* b = (BoundingGround*) ob;

      f32 lowest_z = a->center.z() - a->radius;
      return (lowest_z < b->dataAtPoint(a->center.xy(), NULL)); 
    });

  /*MANIFOLD_FUNC(OBB, GROUND, {

      BoundingOBB* a = (BoundingOBB*) oa;
      BoundingGround* b = (BoundingGround*) ob;

      f32 max_penetration = 0.0f;
      Vec3f cur_normal;
      Vec3f normals[4];
      f32 penetrations[4];
      u8 num_ground_points = 0;

      for (u8 x=0; x<2; ++x) {
	for (u8 y=0; y<2; ++y) {
	  for (u8 z=0; z<2; ++z) {

	    Vec3f point =
	      a->center+a->coord*
	      (Vec3f
	       (x ? a->halves.x() : -a->halves.x(),
		y ? a->halves.y() : -a->halves.y(),
		z ? a->halves.z() : -a->halves.z()));

	    f32 cur_pen = b->dataAtPoint(point.xy(), &cur_normal) - point.z();
	    //Log::message(to_string(b->dataAtPoint(point.xy(), &cur_normal)));

	    if (cur_pen > 0.0f) {
	      
	      max_penetration =
		cur_pen > max_penetration ? cur_pen : max_penetration;
	      penetrations[num_ground_points] = cur_pen;
	      normals[num_ground_points++] = cur_normal;

	      //that's right
	      if (num_ground_points > 3) goto end_loop;
	    }
	  }
	}
      }
    end_loop:

      if (num_ground_points == 0) return false;

      man->normal = Vec3f(0,0,0);
      man->penetration = max_penetration;

      for (u8 n=0; n<num_ground_points; ++n) {
	man->normal -= normals[n]*(penetrations[n]/max_penetration);      
      }

      man->normal.normalize();

      return true;
    
      });*/
  
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
	    return true;
	  }
	}
      }

      return false;
    
    });
}
