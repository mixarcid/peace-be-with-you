#include "Contact.hpp"

NAMESPACE {

  bool SphereVsSphere(BoundingSphere a, BoundingSphere b) {
    //Log::message("testing...");
    //Log::message(a.center.toString() + " " + b.center.toString());
    Vec3f d = a.center - b.center;
    f32 dsq = Vec3f::dot(d,d);
    f32 rsq = sqr(a.radius + b.radius);
    //Log::message("d: " + d.toString());
    //Log::message("rsq: " + to_string(rsq) + "\n");
    return dsq < rsq;
  }
  
  bool SphereVsSphere(BoundingSphere a, BoundingSphere b, Manifold* man) {
    Vec3f d = a.center - b.center;
    f32 dsq = Vec3f::dot(d,d);
    f32 r = a.radius + b.radius;
    f32 rsq = sqr(r);
    
    if (dsq < rsq) {
      f32 dist = sqrt(dsq);
      if (dist != 0) {
	man->penetration = r - dist;
	man->normal = d/dist;
      } else {
	man->penetration = a.radius;
	man->normal = d/dist;
      }
      return true;
    } else {
      return false;
    }
  }

  bool SphereVsOBB(BoundingSphere a, BoundingOBB b) {
    Vec3f d = b.getClosestPoint(a.center) - a.center;
    return Vec3f::dot(d,d) > sqr(a.radius);
  }
  
  bool SphereVsOBB(BoundingSphere a, BoundingOBB b, Manifold* man) {
    return false;
  }

  bool OBBVsSphere(BoundingOBB a, BoundingSphere b) {
    return SphereVsOBB(b, a);
  }
  
  bool OBBVsSphere(BoundingOBB a, BoundingSphere b, Manifold* man) {
    return false;
  }

  const f32 PEACE_OBB_TEST_EPSILON = 0.0001;
  
  bool OBBVsOBB(BoundingOBB a, BoundingOBB b) {
    
    Mat3f r, ar;
    f32 ra, rb;

    r = Mat3f::mulT1(a.coord, b.coord);
    Vec3f t = Mat3f::mulT(a.coord, b.center - a.center);
    
    for (u8 i = 0; i < 3; ++i) {
      for (u8 j = 0; j < 3; ++j) {
	ar.cols[j].data[i] = abs(r(i,j)) + PEACE_OBB_TEST_EPSILON;
      }
    }

    //a's axes
    for (u8 i = 0; i < 3; ++i) {
      ra = a.halves[i];
      rb = Vec3f::dot(ar.row(i), b.halves);
      if (abs(t[i]) > ra + rb) {
	return false;
      }
    }

    //b's axes
    for (u8 i = 0; i < 3; ++i) {
      ra = Vec3f::dot(ar.col(i), a.halves);
	rb = b.halves[i];
	if (abs(Vec3f::dot(t,r.col(i))) > ra + rb) {
	return false;
      }
    }

    //a's edges x b's edges
    ra = a.halves[1] * ar(2,0) + a.halves[2] * ar(1,0);
    rb = b.halves[1] * ar(0,2) + b.halves[2] * ar(0,1);
    if (abs(t[2] * r(1,0) - t[1] * r(2,0))
	> ra + rb) return false;

    ra = a.halves[1] * ar(2,1) + a.halves[2] * ar(1,1);
    rb = b.halves[0] * ar(0,2) + b.halves[2] * ar(0,0);
    if (abs(t[2] * r(1,1) - t[1] * r(2,1))
	> ra + rb) return false;

    ra = a.halves[1] * ar(2,2) + a.halves[2] * ar(1,2);
    rb = b.halves[0] * ar(0,1) + b.halves[1] * ar(0,0);
    if (abs(t[2] * r(1,2) - t[1] * r(2,2))
	> ra + rb) return false;

    ra = a.halves[0] * ar(2,0) + a.halves[2] * ar(0,0);
    rb = b.halves[1] * ar(1,2) + b.halves[2] * ar(1,1);
    if (abs(t[0] * r(2,0) - t[2] * r(0,0))
	> ra + rb) return false;

    ra = a.halves[0] * ar(2,1) + a.halves[2] * ar(0,1);
    rb = b.halves[0] * ar(1,2) + b.halves[2] * ar(1,0);
    if (abs(t[0] * r(2,1) - t[2] * r(0,1))
	> ra + rb) return false;

    ra = a.halves[0] * ar(2,2) + a.halves[2] * ar(0,2);
    rb = b.halves[0] * ar(1,1) + b.halves[1] * ar(1,0);
    if (abs(t[0] * r(2,2) - t[2] * r(0,2))
	> ra + rb) return false;

    ra = a.halves[0] * ar(1,0) + a.halves[1] * ar(0,0);
    rb = b.halves[1] * ar(2,2) + b.halves[2] * ar(2,1);
    if (abs(t[1] * r(0,0) - t[0] * r(1,0))
	> ra + rb) return false;

    ra = a.halves[0] * ar(1,1) + a.halves[1] * ar(0,1);
    rb = b.halves[0] * ar(2,2) + b.halves[2] * ar(2,0);
    if (abs(t[1] * r(0,1) - t[0] * r(1,1))
	> ra + rb) return false;

    ra = a.halves[0] * ar(1,2) + a.halves[1] * ar(0,2);
    rb = b.halves[0] * ar(2,1) + b.halves[1] * ar(2,0);
    if (abs(t[1] * r(0,2) - t[0] * r(1,2))
	> ra + rb) return false;
 
    return true;
  }

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
  
  bool OBBVsOBB(BoundingOBB a,
		BoundingOBB b,
		Manifold* man) {

    //Log::message("\n");
    Mat3f r, ar;
    Vec3f t;
    f32 ra, rb;
    f32 dist;
    f32 pen = -FLT_MAX;
    f32 face_pen = -FLT_MAX;
    f32 face_pen_a = -FLT_MAX;
    u8 axis = 15;
    u8 face_axis = 6;

    r = Mat3f::mulT1(a.coord, b.coord);
    t = Mat3f::mulT(a.coord, b.center - a.center);
    
    for (u8 i = 0; i < 3; ++i) {
      for (u8 j = 0; j < 3; ++j) {
	ar.cols[j].data[i] = abs(r(i,j)) + PEACE_OBB_TEST_EPSILON;
      }
    }

    //a's axes
    for (u8 i = 0; i < 3; ++i) {
      ra = a.halves[i];
      rb = Vec3f::dot(ar.row(i), b.halves);
      dist = abs(t[i]) - (ra + rb);
      //Log::message("%u", i);
      PEACE_OBB_FACE_TEST_A(i)
    }

    //b's axes
    for (u8 i = 0; i < 3; ++i) {
      ra = Vec3f::dot(ar.col(i), a.halves);
      rb = b.halves[i];
      dist = abs(Vec3f::dot(t,r.col(i))) - (ra + rb);
      PEACE_OBB_FACE_TEST_B(i+3)
    }

    //a's edges x b's edges
    //if (index == 15) {
      ra = a.halves[1] * ar(2,0) + a.halves[2] * ar(1,0);
      rb = b.halves[1] * ar(0,2) + b.halves[2] * ar(0,1);
      dist = abs(t[2] * r(1,0) - t[1] * r(2,0))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(6)

      ra = a.halves[1] * ar(2,1) + a.halves[2] * ar(1,1);
      rb = b.halves[0] * ar(0,2) + b.halves[2] * ar(0,0);
      dist = abs(t[2] * r(1,1) - t[1] * r(2,1))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(7)

      ra = a.halves[1] * ar(2,2) + a.halves[2] * ar(1,2);
      rb = b.halves[0] * ar(0,1) + b.halves[1] * ar(0,0);
      dist = abs(t[2] * r(1,2) - t[1] * r(2,2))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(8)

      ra = a.halves[0] * ar(2,0) + a.halves[2] * ar(0,0);
      rb = b.halves[1] * ar(1,2) + b.halves[2] * ar(1,1);
      dist = abs(t[0] * r(2,0) - t[2] * r(0,0))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(9)

      ra = a.halves[0] * ar(2,1) + a.halves[2] * ar(0,1);
      rb = b.halves[0] * ar(1,2) + b.halves[2] * ar(1,0);
      dist = abs(t[0] * r(2,1) - t[2] * r(0,1))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(10)

      ra = a.halves[0] * ar(2,2) + a.halves[2] * ar(0,2);
      rb = b.halves[0] * ar(1,1) + b.halves[1] * ar(1,0);
      dist = abs(t[0] * r(2,2) - t[2] * r(0,2))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(11)
    
      ra = a.halves[0] * ar(1,0) + a.halves[1] * ar(0,0);
      rb = b.halves[1] * ar(2,2) + b.halves[2] * ar(2,1);
      dist = abs(t[1] * r(0,0) - t[0] * r(1,0))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(12)
    
      ra = a.halves[0] * ar(1,1) + a.halves[1] * ar(0,1);
      rb = b.halves[0] * ar(2,2) + b.halves[2] * ar(2,0);
      dist = abs(t[1] * r(0,1) - t[0] * r(1,1))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(13)
    
      ra = a.halves[0] * ar(1,2) + a.halves[1] * ar(0,2);
      rb = b.halves[0] * ar(2,1) + b.halves[1] * ar(2,0);
      dist = abs(t[1] * r(0,2) - t[0] * r(1,2))
	- (ra + rb);
      PEACE_OBB_EDGE_TEST(14)
	//}

    Vec3f norm;
    debugAssert(axis < 15,
		"OBB vs OBB axis should be less than 15");
    debugAssert(face_axis < 6,
		"OBB vs OBB face axis should be less than 6");
    switch (axis) {
    case 0: case 1: case 2:
      norm = a.coord.col(axis);
      //Log::message("axis: %u", axis);
      break;
    case 3: case 4: case 5:
      norm = b.coord.col(axis-3);
      //Log::message("axis: %u", axis);
      break;
    default:
      //Log::message("%u,%u,%u", axis, (axis-6)/3, (axis-6)%3);
      //Log::message(to_string(a.coord) + to_string(b.coord));
      norm = Vec3f::cross(b.coord.col((axis-6)/3),
			  a.coord.col((axis-6)%3));
      //Log::message(to_string(norm.norm()));
      if (norm.norm() < 0.1) {
	pen = face_pen;
	//Log::message("!!!!!!!!!!!!!!");
	if (face_axis < 3) {
	  norm = a.coord.col(face_axis);
	} else {
	  norm = b.coord.col(face_axis-3);
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
  }

  bool testIntersection(BoundingObject a, BoundingObject b) {
    switch (a.type) {
    case BOUNDING_SPHERE:
      switch(b.type) {
      case BOUNDING_SPHERE:
	return SphereVsSphere(a.sphere, b.sphere);
      case BOUNDING_OBB:
	return SphereVsOBB(a.sphere, b.obb);
      case BOUNDING_NONE:
	return false;
      }
    case BOUNDING_OBB:
      switch(b.type) {
      case BOUNDING_SPHERE:
	return OBBVsSphere(a.obb, b.sphere);
      case BOUNDING_OBB:
	return OBBVsOBB(a.obb, b.obb);
      case BOUNDING_NONE:
	return false;
      }
    case BOUNDING_NONE:
      return false;
    }
    return false;
  }

  bool testIntersection(BoundingObject a,
			BoundingObject b,
			Manifold* man) {
    switch (a.type) {
    case BOUNDING_SPHERE:
      switch(b.type) {
      case BOUNDING_SPHERE:
	return SphereVsSphere(a.sphere, b.sphere, man);
      case BOUNDING_OBB:
	return SphereVsOBB(a.sphere, b.obb, man);
      case BOUNDING_NONE:
	return false;
      }
    case BOUNDING_OBB:
      switch(b.type) {
      case BOUNDING_SPHERE:
	return OBBVsSphere(a.obb, b.sphere, man);
      case BOUNDING_OBB:
	return OBBVsOBB(a.obb, b.obb, man);
      case BOUNDING_NONE:
	return false;
      }
    case BOUNDING_NONE:
      return false;
    }
    return false;
  }
  
}
