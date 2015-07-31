#pragma once

#include "Math.hpp"
#include "GL.hpp"
#include "Standard.hpp"

NAMESPACE {

  const float VEC_UNIT_MAX_ERROR = 0.01;

  template <typename T>
    struct Vec2 {

    T x;
    T y;
    
    Vec2(T xval = 0, T yval = 0) : x(xval), y(yval) {}

    void operator=(const Vec2 b) {
      memcpy(this, &b, sizeof(Vec2<T>));
    }

    Vec2 operator+(const Vec2 b) {
      return Vec2(x + b.x,
		  y + b.y);
    }
    
    Vec2 operator-(const Vec2 b) {
      return Vec2(x - b.x,
		  y - b.y);
    }

    String toString() {
      return "<" + to_string(x) + ", "
	+ to_string(y) + ">";
    }
  };

  typedef Vec2<f32> Vec2f;
  
  
  template <typename T>
    struct Vec3 {
    
    T x;
    T y;
    T z;

    Vec3(T xval = 0 , T yval = 0 , T zval = 0)
      : x(xval), y(yval), z(zval) {}

    void operator=(const Vec3 b) {
      memcpy(this, &b, sizeof(Vec3<T>));
    }

    Vec3 operator*(const T b) const {
      return Vec3(x*b,
		  y*b,
		  z*b);
    }

    Vec3 operator/(const T b) const {
      return Vec3(x/b,
		  y/b,
		  z/b);
    }
	
    Vec3 operator+(const Vec3 b) const {
      return Vec3(x + b.x,
		  y + b.y,
		  z + b.z);
    }
      
    Vec3 operator-(const Vec3 b) const {
      return Vec3(x - b.x,
		  y - b.y,
		  z - b.z);
    }

    Vec3 operator-() const {
      return Vec3(-x, -y, -z);
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

    T operator[](const u8 index) {
      debugAssert(index < 3,
		  "Vec3 index must be 0 to 2");
      return (*((T*)this + sizeof(T)*index)); //that's right
    }
    
    T abs() const {
      return sqrt(sqr(x) + sqr(y) + sqr(z));
    }

    bool isUnit() const {
      T mag = abs();
      return abs(mag - 1) < VEC_UNIT_MAX_ERROR;
    }

    void makeUnit() {
      (*this) = (*this)/abs();
    }

    Vec3 getUnit() const {
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

    static Vec3 lerp(Vec3 a, Vec3 b, T h) {
      debugAssert(h >= 0.0 && h <= 1.0,
		  "H must be between 0 and 1 for Vec3 lerp");
      return a*(1-h) + b*h;
    }

    static Vec3 slerp(Vec3 a, Vec3 b, T h) {
      
      debugAssert(h > 0.0 && h < 1.0,
		  "H must be between 0 and 1 for Vec3 slerp");
      T cos_half_angle = Vec3::dot(a,b);
      
      if (fabs(cos_half_angle) >= 1.0f) {
	return a;
      }
      
      T half_angle = acos(cos_half_angle);
      T sin_half_angle = sqrt(1 - sqr(cos_half_angle));
      T r1 = sin((1 - h) * half_angle)/sin_half_angle;
      T r2 = sin(h * half_angle)/sin_half_angle;
      
      return Vec3(a.x*r1 + b.x*r2,
		  a.y*r1 + b.y*r2,
		  a.z*r1 + b.z*r2);
    }
    
  };
  
  typedef Vec3<f32> Vec3f;

  template <typename T>
    struct Mat4 {

    T data[16];

    Mat4() {
      T d[16] = {1,0,0,0,
		 0,1,0,0,
		 0,0,1,0,
		 0,0,0,1};
      memcpy(this, d, sizeof(Mat4<T>));
    }

    Mat4(const T* initial_data) {
      /*for (int n = 0; n < 16; ++n) {
	data[n] = initial_data[n];
	}*/
      memcpy(this, initial_data, sizeof(Mat4<T>));
    }

    /*Mat4(Mat4 b) {
      (*this) = b;
      }*/

    T operator()(u8 row, u8 col) const {
      debugAssert(row < 4 && col < 4, "Matrix index out of bounds");
      return data[(row*4) + col];
    }

    void operator=(const Mat4 b) {
      memcpy(this, &b, sizeof(Mat4<T>));
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
      for (u32 row=0; row<4; ++row) {
	for (u32 col=0; col<4; ++col) {
	  ret.data[row*4 + col] = 0;
	  for (u32 n=0; n<4; ++n) {
	    ret.data[row*4 + col] += (*this)(row, n)*(b(n, col));
	  }
	}
      }
      return ret;
    }

    void operator*=(const Mat4 b) {
      (*this) = (*this) * b;
    }

    /*void translate(Vec3<T> trans) {
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
      }*/

    void makeIdentity() {
      (*this) = Mat4();
    }

    String toString() {
      String ret = "\n";
      for (u8 row = 0; row < 4; ++row) {
	ret += "| ";
	for (u8 col = 0; col < 4; ++col) {
	  ret += to_string((*this)(col, row)) + " ";
	}
	ret += "|";
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
      //Log::message("%f",fovy);
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

  typedef Mat4<f32> Mat4f;

  template <typename T>
    struct Mat3 {

    union {
      struct {
	Vec3<T> rows[3];
      };
      T data[9];
    };

    Mat3() {
      T d[9] = {1,0,0,
		0,1,0,
		0,0,1};
      memcpy(this, d, sizeof(Mat3<T>));
    }

    Mat3(const T* initial_data) {
      memcpy(this, initial_data, sizeof(Mat3<T>));
    }

    Vec3<T> vecMul(Vec3<T> vec) {
      return Vec3<T>(vec.x*data[0] + vec.y*data[3] + vec.z*data[6],
		     vec.x*data[1] + vec.y*data[4] + vec.z*data[7],
		     vec.x*data[2] + vec.y*data[5] + vec.z*data[8]);
    }

    Vec3<T> vecMulT(Vec3<T> vec) {
      return Vec3<T>(Vec3<T>::dot(vec,rows[0]),
		     Vec3<T>::dot(vec,rows[1]),
		     Vec3<T>::dot(vec,rows[2]));
    }
    
    T operator()(u8 row, u8 col) const {
      debugAssert(row < 3 && col < 3, "Matrix index out of bounds");
      return data[(row*3) + col];
    }

    Vec3<T> operator[](u8 row) const {
      debugAssert(row < 3, "Matrix index out of bounds");
      return rows[row];
    }

    void operator=(const Mat3 b) {
      memcpy(this, &b, sizeof(Mat3<T>));
    }

    Mat3 operator+(const Mat3 b) {
      Mat3 ret;
      for (int n = 0; n < 9; ++n) {
	ret[n] = data[n] + b.data[n];
      }
      return ret;
    }

    Mat3 operator*(const Mat3 b) {
      Mat3 ret;
      for (u32 row=0; row<3; ++row) {
	for (u32 col=0; col<3; ++col) {
	  ret.data[row*3 + col] = 0;
	  for (u32 n=0; n<3; ++n) {
	    ret.data[row*3 + col] += (*this)(row, n)*(b(n, col));
	  }
	}
      }
      return ret;
    }

    void operator*=(const Mat3 b) {
      (*this) = (*this) * b;
    }

    void makeIdentity() {
      (*this) = Mat3();
    }

    String toString() {
      String ret = "\n";
      for (u8 row = 0; row < 3; ++row) {
	ret += "| ";
	for (u8 col = 0; col < 3; ++col) {
	  ret += to_string((*this)(col, row)) + " ";
	}
	ret += "|";
      }
      return ret;
    }
    
  };

  typedef Mat3<f32> Mat3f;
  
}
