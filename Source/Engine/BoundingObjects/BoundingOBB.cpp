#include "BoundingOBB.hpp"
#include "BoundingAABB.hpp"
#include "Manifold.hpp"
#include "RenderableShape.hpp"

NAMESPACE {

  const f32 PEACE_OBB_TEST_EPSILON = 0.0001;

  BoundingOBB::BoundingOBB(Array<BasicMeshData>& data) {
    
    //just computes the AABB, currently
    debugAssert(data.size() > 0,
		"Why does your mesh have no data?");
    Vec3f max, min = data[0].pos;

    for (BasicMeshData d : data) {
      max.x() = max.x() > d.pos.x() ? max.x() : d.pos.x();
      max.y() = max.y() > d.pos.y() ? max.y() : d.pos.y();
      max.z() = max.z() > d.pos.z() ? max.z() : d.pos.z();
      min.x() = min.x() < d.pos.x() ? min.x() : d.pos.x();
      min.y() = min.y() < d.pos.y() ? min.y() : d.pos.y();
      min.z() = min.z() < d.pos.z() ? min.z() : d.pos.z();
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

  BoundingObject* BoundingOBB::transform(TransformBasic t) {
    BoundingOBB* ret = new BoundingOBB(*this);
    ret->center += t.trans;
    ret->coord *= t.rot.mat3();
    //Log::message(to_string(ret->coord));
    return ret;
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

  void BoundingOBB::render(RenderContext c) {
    Shader::UNI_MODEL.registerVal
      (Mat4f::translate(center) *
       Mat4f(coord) *
       Mat4f::scale(halves));
    RenderableShape::CUBE.render(c);
  }
  
  CONTAINED_IN_FUNC(OBB, AABB, {

      BoundingOBB* obb = (BoundingOBB*) oa;
      BoundingAABB* aabb = (BoundingAABB*) ob;

      for (u8 x=0; x<2; ++x) {
	for (u8 y=0; x<2; ++x) {
	  for (u8 z=0; x<2; ++x) {
	  
	    Vec3f unoriented_vert(x ? obb->halves.x() : -obb->halves.x(),
				  y ? obb->halves.y() : -obb->halves.y(),
				  z ? obb->halves.z() : -obb->halves.z());
	    Vec3f vert = obb->center + (obb->coord*unoriented_vert);
	    Vec3f rel_center = aabb->center - vert;
	  
	    if (abs(rel_center.x()) > aabb->halves.x() ||
		abs(rel_center.y()) > aabb->halves.y() ||
		abs(rel_center.z()) > aabb->halves.z()) return false;
	  
	  }
	}
      }
      return true;
      
    });

  COLLIDE_FUNC(OBB, OBB, {

      BoundingOBB* a = (BoundingOBB*) oa;
      BoundingOBB* b = (BoundingOBB*) ob;
    
      Mat3f r, ar;
      f32 ra, rb;

      r = Mat3f::mulT1(a->coord, b->coord);
      Vec3f t = Mat3f::mulT(a->coord, b->center - a->center);
    
      for (u8 i = 0; i < 3; ++i) {
	for (u8 j = 0; j < 3; ++j) {
	  ar.cols[j].data[i] = abs(r(i,j)) + PEACE_OBB_TEST_EPSILON;
	}
      }

      //a's axes
      for (u8 i = 0; i < 3; ++i) {
	ra = a->halves[i];
	rb = Vec3f::dot(ar.row(i), b->halves);
	if (abs(t[i]) > ra + rb) {
	  return false;
	}
      }

      //b's axes
      for (u8 i = 0; i < 3; ++i) {
	ra = Vec3f::dot(ar.col(i), a->halves);
	rb = b->halves[i];
	if (abs(Vec3f::dot(t,r.col(i))) > ra + rb) {
	  return false;
	}
      }

      //a's edges x b's edges
      ra = a->halves[1] * ar(2,0) + a->halves[2] * ar(1,0);
      rb = b->halves[1] * ar(0,2) + b->halves[2] * ar(0,1);
      if (abs(t[2] * r(1,0) - t[1] * r(2,0))
	  > ra + rb) return false;

      ra = a->halves[1] * ar(2,1) + a->halves[2] * ar(1,1);
      rb = b->halves[0] * ar(0,2) + b->halves[2] * ar(0,0);
      if (abs(t[2] * r(1,1) - t[1] * r(2,1))
	  > ra + rb) return false;

      ra = a->halves[1] * ar(2,2) + a->halves[2] * ar(1,2);
      rb = b->halves[0] * ar(0,1) + b->halves[1] * ar(0,0);
      if (abs(t[2] * r(1,2) - t[1] * r(2,2))
	  > ra + rb) return false;

      ra = a->halves[0] * ar(2,0) + a->halves[2] * ar(0,0);
      rb = b->halves[1] * ar(1,2) + b->halves[2] * ar(1,1);
      if (abs(t[0] * r(2,0) - t[2] * r(0,0))
	  > ra + rb) return false;

      ra = a->halves[0] * ar(2,1) + a->halves[2] * ar(0,1);
      rb = b->halves[0] * ar(1,2) + b->halves[2] * ar(1,0);
      if (abs(t[0] * r(2,1) - t[2] * r(0,1))
	  > ra + rb) return false;

      ra = a->halves[0] * ar(2,2) + a->halves[2] * ar(0,2);
      rb = b->halves[0] * ar(1,1) + b->halves[1] * ar(1,0);
      if (abs(t[0] * r(2,2) - t[2] * r(0,2))
	  > ra + rb) return false;

      ra = a->halves[0] * ar(1,0) + a->halves[1] * ar(0,0);
      rb = b->halves[1] * ar(2,2) + b->halves[2] * ar(2,1);
      if (abs(t[1] * r(0,0) - t[0] * r(1,0))
	  > ra + rb) return false;

      ra = a->halves[0] * ar(1,1) + a->halves[1] * ar(0,1);
      rb = b->halves[0] * ar(2,2) + b->halves[2] * ar(2,0);
      if (abs(t[1] * r(0,1) - t[0] * r(1,1))
	  > ra + rb) return false;

      ra = a->halves[0] * ar(1,2) + a->halves[1] * ar(0,2);
      rb = b->halves[0] * ar(2,1) + b->halves[1] * ar(2,0);
      if (abs(t[1] * r(0,2) - t[0] * r(1,2))
	  > ra + rb) return false;
 
      return true;
    });

#define PEACE_OBB_FACE_TEST_A(index)		\
  if (dist > 0) {				\
    return false;				\
  } else if (dist > pen) {			\
    face_pen_a = face_pen = pen = dist;		\
    face_axis = axis = index;			\
    /*Log::message("%u,%f", index, dist);*/	\
  }

#define PEACE_OBB_FACE_TEST_B(index)		\
  if (dist > 0) {				\
    return false;				\
  } else if (dist > face_pen_a /*+0.0001*/ &&	\
	     dist > pen) {			\
    face_pen = pen = dist;			\
    face_axis = axis = index;			\
    /*Log::message("%u,%f", index, dist);*/	\
  }

#define PEACE_OBB_EDGE_TEST(index)		\
  if (dist > 0) {				\
    return false;				\
  } else if (dist > face_pen+0.5 &&		\
	     dist > pen) {			\
    pen = dist;					\
    axis = index;				\
    /*Log::message("%u,%f", index, dist);*/	\
  }
  
  MANIFOLD_FUNC(OBB, OBB, {

      BoundingOBB* a = (BoundingOBB*) oa;
      BoundingOBB* b = (BoundingOBB*) ob;
    
      Mat3f r, ar;
      Vec3f t;
      f32 ra, rb;
      f32 dist;
      f32 pen = -FLT_MAX;
      f32 face_pen = -FLT_MAX;
      f32 face_pen_a = -FLT_MAX;
      u8 axis = 15;
      u8 face_axis = 6;

      r = Mat3f::mulT1(a->coord, b->coord);
      t = Mat3f::mulT(a->coord, b->center - a->center);
    
      for (u8 i = 0; i < 3; ++i) {
	for (u8 j = 0; j < 3; ++j) {
	  ar.cols[j].data[i] = abs(r(i,j)) + PEACE_OBB_TEST_EPSILON;
	}
      }

      //a's axes
      for (u8 i = 0; i < 3; ++i) {
	ra = a->halves[i];
	rb = Vec3f::dot(ar.row(i), b->halves);
	dist = abs(t[i]) - (ra + rb);
	//Log::message("%u", i);
	PEACE_OBB_FACE_TEST_A(i);
      }

      //b's axes
      for (u8 i = 0; i < 3; ++i) {
	ra = Vec3f::dot(ar.col(i), a->halves);
	rb = b->halves[i];
	dist = abs(Vec3f::dot(t,r.col(i))) - (ra + rb);
	PEACE_OBB_FACE_TEST_B(i+3);
      }

      //a's edges x b's edges
      //if (index == 15) {
      ra = a->halves[1] * ar(2,0) + a->halves[2] * ar(1,0);
      rb = b->halves[1] * ar(0,2) + b->halves[2] * ar(0,1);
      dist = abs(t[2] * r(1,0) - t[1] * r(2,0))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(6);

      ra = a->halves[1] * ar(2,1) + a->halves[2] * ar(1,1);
      rb = b->halves[0] * ar(0,2) + b->halves[2] * ar(0,0);
      dist = abs(t[2] * r(1,1) - t[1] * r(2,1))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(7);

      ra = a->halves[1] * ar(2,2) + a->halves[2] * ar(1,2);
      rb = b->halves[0] * ar(0,1) + b->halves[1] * ar(0,0);
      dist = abs(t[2] * r(1,2) - t[1] * r(2,2))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(8);

      ra = a->halves[0] * ar(2,0) + a->halves[2] * ar(0,0);
      rb = b->halves[1] * ar(1,2) + b->halves[2] * ar(1,1);
      dist = abs(t[0] * r(2,0) - t[2] * r(0,0))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(9);

      ra = a->halves[0] * ar(2,1) + a->halves[2] * ar(0,1);
      rb = b->halves[0] * ar(1,2) + b->halves[2] * ar(1,0);
      dist = abs(t[0] * r(2,1) - t[2] * r(0,1))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(10);

      ra = a->halves[0] * ar(2,2) + a->halves[2] * ar(0,2);
      rb = b->halves[0] * ar(1,1) + b->halves[1] * ar(1,0);
      dist = abs(t[0] * r(2,2) - t[2] * r(0,2))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(11);
    
      ra = a->halves[0] * ar(1,0) + a->halves[1] * ar(0,0);
      rb = b->halves[1] * ar(2,2) + b->halves[2] * ar(2,1);
      dist = abs(t[1] * r(0,0) - t[0] * r(1,0))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(12);
    
      ra = a->halves[0] * ar(1,1) + a->halves[1] * ar(0,1);
      rb = b->halves[0] * ar(2,2) + b->halves[2] * ar(2,0);
      dist = abs(t[1] * r(0,1) - t[0] * r(1,1))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(13);
    
      ra = a->halves[0] * ar(1,2) + a->halves[1] * ar(0,2);
      rb = b->halves[0] * ar(2,1) + b->halves[1] * ar(2,0);
      dist = abs(t[1] * r(0,2) - t[0] * r(1,2))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(14);
      //}

      Vec3f norm;
      debugAssert(axis < 15,
		  "OBB vs OBB axis should be less than 15");
      debugAssert(face_axis < 6,
		  "OBB vs OBB face axis should be less than 6");
      switch (axis) {
      case 0: case 1: case 2:
	norm = a->coord.col(axis);
	//Log::message("axis: %u", axis);
	break;
      case 3: case 4: case 5:
	norm = b->coord.col(axis-3);
	//Log::message("axis: %u", axis);
	break;
      default:
	//Log::message("%u,%u,%u", axis, (axis-6)/3, (axis-6)%3);
	//Log::message(to_string(a->coord) + to_string(b->coord));
	norm = Vec3f::cross(b->coord.col((axis-6)/3),
			    a->coord.col((axis-6)%3));
	//Log::message(to_string(norm.norm()));
	if (norm.norm() < 0.1) {
	  pen = face_pen;
	  //Log::message("!!!!!!!!!!!!!!");
	  if (face_axis < 3) {
	    norm = a->coord.col(face_axis);
	  } else {
	    norm = b->coord.col(face_axis-3);
	  }
	  //Log::message("axis: %u", face_axis);
	} /*else {
	    Log::message("axis: %u", axis);
	    }*/
	break;
      }

      /*f32 norm_len = norm.norm();
	if (norm_len < 0.001) {
	norm = Vec3f(1,0,0);
	Log::error("Why is the OBB vs OBB normal the zero vector?");
	} else {
	norm /= norm_len;
	}*/
      norm.normalize();
      man->penetration = pen;
      man->normal = norm;

      //Log::message(to_string(norm));
    
      return true;
    });

}
