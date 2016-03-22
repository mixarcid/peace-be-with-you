#include "BoundingFrustum.hpp"
#include "BoundingAABB.hpp"
#include "BoundingAABB2D.hpp"
#include "BoundingSphere.hpp"

NAMESPACE {

  BoundingFrustum::BoundingFrustum(Mat4f mvp) {
    for (u8 n=0; n<3; ++n) {
      for (u8 s=0; s<2; ++s) {
	u8 index = 2*n+s;
	planes[index] = mvp.row(3) +
	  (s ? mvp.row(n) : -mvp.row(n));
	planes[index]/=(planes[index].xyz().norm());
      }
    }
  }

  //Thanks, txutxi, for the more efficient method
  COLLIDE_FUNC(FRUSTUM, AABB, {

      BoundingFrustum* a = (BoundingFrustum*) oa;
      BoundingAABB* b = (BoundingAABB*) ob;
      Vec3f aabb_bounds[2] = {
	b->center - b->halves,
	b->center + b->halves
      };

      for (u8 n=0; n<6; ++n) {
	
	Vec4f plane = a->planes[n];

	u8 px = (plane.x() > 0);
	u8 py = (plane.y() > 0);
	u8 pz = (plane.z() > 0);

	f32 dot = Vec3f::dot
	  (plane.xyz(),
	   Vec3f(aabb_bounds[px].x(),
		 aabb_bounds[py].y(),
		 aabb_bounds[pz].z()));
	if (dot < -plane.w()) return false;
      }
      return true;
    });

  COLLIDE_FUNC(FRUSTUM, AABB2D, {

      BoundingFrustum* a = (BoundingFrustum*) oa;
      BoundingAABB2D* b = (BoundingAABB2D*) ob;

      //BoundingAABB c(Vec3f(b->center, 0), Vec3f(b->halves, 10000));
      //return c.intersects(a);
      Vec2f aabb_bounds[2] = {
	b->center - b->halves,
	b->center + b->halves
      };

      for (u8 n=0; n<6; ++n) {
	
	Vec4f plane = a->planes[n];

	u8 px = (plane.x() > 0);
	u8 py = (plane.y() > 0);

	f32 dot = Vec2f::dot
	  (plane.xy(),
	   Vec2f(aabb_bounds[px].x(),
		 aabb_bounds[py].y()));
	if (dot < -plane.w()) return false;
      }
      return true;
    });

  COLLIDE_FUNC(FRUSTUM, SPHERE, {

      BoundingFrustum* a = (BoundingFrustum*) oa;
      BoundingSphere* b = (BoundingSphere*) ob;

      for (u8 n=0; n<6; ++n) {
	
	Vec4f plane = a->planes[n];

	f32 dot = Vec3f::dot
	  (plane.xyz(), b->center);
	f32 dist = (dot+plane.w());
	if (dist < -b->radius) return false;
      }
      return true;
    });
  
}
