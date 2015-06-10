#pragma once

#include <math.h>
#include <string.h>
#include "Graphics.hpp"
#include "Standard.hpp"

NAMESPACE {

  template<typename T>
    inline T degreesToRadians(T deg) {
    return deg*(M_PI/(T)180);
  }

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

  typedef Vec2<GLfloat> Vec2f;
  
  
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

    Vec3() {
      x = 0;
      y = 0;
      z = 0;
    }

    void operator=(const Vec3 b) {
      memcpy(this, &b, 3*sizeof(T));
    }

    Vec3 operator*(const T b) {
      return Vec3(x*b,
		  y*b,
		  z*b);
    }

    Vec3 operator/(const T b) {
      return Vec3(x/b,
		  y/b,
		  z/b);
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

    Vec3 operator+=(const Vec3 b) {
      return Vec3(x += b.x,
		  y += b.y,
		  z += b.z);
    }
      
    Vec3 operator-=(const Vec3 b) {
      return Vec3(x -= b.x,
		  y -= b.y,
		  z -= b.z);
    }
    
    T abs() {
      return sqrt(sqr(x) + sqr(y) + sqr(z));
    }

    const T UNIT_MAX_ERROR = 0.01;
    bool isUnit() {
      T mag = abs();
      return mag < 1.0f + UNIT_MAX_ERROR
		   && mag > 1.0f - UNIT_MAX_ERROR;
    }

    void makeUnit() {
      (*this) = (*this)/abs();
    }

    Vec3 getUnit() {
      return (*this)/abs();
    }

    String toString() {
      return "<" + to_string(x) + ", "
	+ to_string(y) + ", "
	+ to_string(z) + ">";
    }

    static Vec3 cross(Vec3 v1, Vec3 v2) {
	return Vec3((v1.y*v2.z) - (v1.z*v2.y),
		    (v1.z*v2.x) - (v1.x*v2.z),
		    (v1.x*v2.y) - (v1.y*v2.x));
      }

    static T dot(Vec3 v1, Vec3 v2) {
      return (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z); 
    }
    
  };
  
  typedef Vec3<GLfloat> Vec3f;

  template <typename T>
    struct Mat4 {
    
    T data[16];

    Mat4() {
      T d[16] = {1,0,0,0,
		 0,1,0,0,
		 0,0,1,0,
		 0,0,0,1};
      memcpy(this, d, 16*sizeof(T));
    }

    Mat4(const T* initial_data) {
      /*for (int n = 0; n < 16; ++n) {
	data[n] = initial_data[n];
	}*/
      memcpy(this, initial_data, 16*sizeof(T));
    }

    /*Mat4(Mat4 b) {
      (*this) = b;
      }*/

    T operator()(unsigned int row, unsigned int col) const {
      debugAssert(row < 4 && col < 4, "Matrix index out of bounds");
      return data[(row*4) + col];
    }

    void operator=(const Mat4 b) {
      memcpy(this, &b, 16*sizeof(T));
    }

    Mat4 operator+(const Mat4 b) {
      Mat4 ret;
      for (int n = 0; n < 16; ++n) {
	ret[n] = data[n] + b.data[n];
      }
      return ret;
    }

    Mat4 operator*(const Mat4 b) {
      Mat4 ret;
      for (unsigned int row=0; row<4; ++row) {
	for (unsigned int col=0; col<4; ++col) {
	  ret.data[row*4 + col] = 0;
	  for (unsigned int n=0; n<4; ++n) {
	    ret.data[row*4 + col] += (*this)(row, n)*(b(n, col));
	  }
	}
      }
      return ret;
    }

    void operator*=(const Mat4 b) {
      (*this) = (*this) * b;
    }

    void translate(Vec3<T> trans) {
      data[12] += data[0]*trans.x +
	data[4]*trans.y + data[8]*trans.z;
      
      data[13] += data[1]*trans.x +
	data[5]*trans.y + data[9]*trans.z;
      
      data[14] += data[2]*trans.x +
	data[6]*trans.y + data[10]*trans.z;
      
      data[15] += data[3]*trans.x +
	data[7]*trans.y + data[11]*trans.z;
    }

    //angle is in radians
    void rotate (Vec3<T> u, T angle) {
      if (angle != 0) {
	(*this) *= Mat4::makeRotate(u, angle);
      }
    }
    
    void scale(Vec3<T> scal) {
      data[0] *= scal.x;
      data[5] *= scal.y;
      data[10] *= scal.z;
    }

    void makeIdentity() {
      (*this) = Mat4();
    }

    String toString() {
      String ret = "\n";
      for (unsigned int row = 0; row < 4; ++row) {
	ret += "| ";
	for (unsigned int col = 0; col < 4; ++col) {
	  ret += to_string((*this)(col, row)) + " ";
	}
	ret += "|\n";
      }
      return ret;
    }

    static Mat4 translate(Mat4 mat, Vec3<T> trans) {
      Mat4 ret(mat);
      ret.translate(trans);
      return ret;
    }

    static Mat4 rotate(Mat4 mat, Vec3<T> u, T angle) {
      return mat * makeRotate(u, angle);
    }
    
    static Mat4 scale(Mat4 mat, Vec3<T> scal) {
      Mat4 ret(mat);
      ret.scale(scal);
      return ret;
    }

    static Mat4 makeTranslate(Vec3<T> trans) {
      T data[16] = {1,0,0,0,
		    0,1,0,0,
		    0,0,1,0,
		    trans.x,trans.y,trans.z,1};
      return Mat4(data);
    }

    static Mat4 makeScale(Vec3<T> scal) {
      T data[16] = {scal.x,0,0,0,
		    0,scal.y,0,0,
		    0,0,scal.z,0,
		    0,0,0,1};
      return Mat4(data);
    }

    //angle is in radians
    static Mat4 makeRotate(Vec3<T> u, T angle) {
      debugAssert(u.isUnit(),
		  "It's advisable to only rotate by a unit vector");
      
      T c = cos(angle);
      T s = sin(angle);
      T l_c = 1-cos(angle);

      T data[16] = {
	u.x*u.x + (1 - u.x*u.x)*c, u.x*u.y*l_c + u.z*s,
	u.x*u.z*l_c - u.y*s,  0,
	u.x*u.y*l_c - u.z*s, u.y*u.y+(1 - u.y*u.y)*c,
	u.y*u.z*l_c + u.x*s,  0,
	u.x*u.z*l_c + u.y*s, u.y*u.z*l_c - u.x*s,
	u.z*u.z + (1 - u.z*u.z)*c,  0,
	0, 0, 0, 1
      };
      return Mat4(data);
    }

    //fovy is in radians
    static Mat4 perspective(T fovy, T aspect, T z_near, T z_far) {
      T sub = z_near - z_far;
      //log::message("%f",fovy);
      T f = 1/tan(fovy/(T)2);
      T data[16] = {
	f/aspect, 0, 0, 0,
	0, f, 0, 0,
	0, 0, (z_far + z_near)/sub, -1,
	0, 0, (2*z_near*z_far)/sub, 0
      };
      return Mat4(data);
    }

    static Mat4 lookAt(Vec3<T> eye, Vec3<T> center, Vec3<T> up) {
      Vec3<T> f = (center - eye).getUnit();
      Vec3<T> s = (Vec3f::cross(f, up)).getUnit();
      Vec3<T> u = Vec3f::cross(s, f); 
      T data[16] = {
	s.x, u.x, -f.x, 0,
	s.y, u.y, -f.y, 0,
	s.z, u.z, -f.z, 0,
	-Vec3f::dot(s, eye), -Vec3f::dot(u, eye),
	Vec3f::dot(f, eye), 1
      };
      return Mat4(data);
    }
  };

  typedef Mat4<GLfloat> Mat4f;
  
}
