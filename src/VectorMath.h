#pragma once

#include <math.h>
#include "Standard.h"

NAMESPACE {

  template<typename T>
  inline T sqr(T x) {
    return x*x;
  }

  template <typename T>
  struct Vec2 {

    T x;
    T y;
    
    Vec2(T xval, T yval) {
      x = xval;
      y = yval;
    }

    Vec2 operator+(const Vec2 b) {
      return Vec2(x + b.x,
		  y + b.y);
    }
    
    Vec2 operator-(const Vec2 b) {
      return Vec2(x - b.x,
		  y - b.y);
    }
  };

  typedef Vec2<float> Vec2f;
  typedef Vec2<double> Vec2d;
  
  
  template <typename T>
  struct Vec3 {
    
    T x;
    T y;
    T z;

    Vec3(T xval, T yval, T zval) {
      x = xval;
      y = yval;
      z = zval;
    }

    Vec3 operator*(const T b) {
      return Vec3(x*b,
		  y*b,
		  z*b);
    }
	
    Vec3 operator+(const Vec3 b) {
	return Vec3(x + b.x,
		    y + b.y,
		    z + b.z);
    }
      
    Vec3 operator-(const Vec3 b) {
      return Vec3(x - b.x,
		  y - b.y,
		  z - b.z);
    }
    
    T abs() {
      return sqrt(sqr(x) + sqr(y) + sqr(z));
    }

    String toString() {
	  return "<" + to_string(x) + ", "
	    + to_string(y) + ", "
	    + to_string(z) + ">";
    }
  };

  template <typename T>
  Vec3<T> cross(Vec3<T> v1, Vec3<T> v2) {
    return Vec3<T>((v1.y*v2.z) - (v1.z*v2.y),
		   (v1.z*v2.x) - (v1.x*v2.z),
		   (v1.x*v2.y) - (v1.y*v2.x));
  }

  template <typename T>
  T dot(Vec3<T> v1, Vec3<T> v2) {
    return (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z); 
  }
  
  typedef Vec3<float> Vec3f;
  typedef Vec3<double> Vec3d;

  /*template <typename T>
  struct Mat4 {
    
    T data[16];

    Mat4() {
      data = {1,0,0,0,
	      0,1,0,0,
	      0,0,1,0,
	      0,0,0,1};
    }

    Mat4(const T* initial_data) {
      for (int n = 0; n < 16; ++n) {
	data[n] = initial_data[n];
      }
    }

    T operator[](unsigned int row, unsigned int col) {
      assert(row < 4 && col < 4, "Matrix index out of bounds");
      return data((row*4) + col);
    }

    Mat4 operator+(const Mat4 b) {
      Mat4 ret;
      for (int n = 0; n < 16; ++n) {
	ret[n] = data[n] + n.data[n];
      }
      return ret;
    }

    void translate(Vec3 trans) {
      data[3] += data[0]*trans.x + data[1]*trans.y + data[2]*trans.z;
      data[7] += data[4]*trans.x + data[5]*trans.y + data[6]*trans.z;
      data[11] += data[8]*trans.x + data[9]*trans.y + data[10]*trans.z;
      data[15] += data[12]*trans.x + data[13]*trans.y + data[14]*trans.z;
    }

    void rotate(Vec3 rot) {

    }

    void scale(Vec3 scal) {
      data[0] *= scal.x;
      data[5] *= scal.y;
      data[10] *= scal.z;
      }
      }*/
}
