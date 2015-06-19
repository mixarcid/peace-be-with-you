#pragma once

#include "VectorMath.hpp"

NAMESPACE {

  template <typename T>
    struct Quaternion {

    T x;
    T y;
    T z;
    T w;

    Quaternion(T qx = 0, T qy = 0, T qz = 0, T qw = 1) :
      x(qx), y(qy), z(qz), w(qw) {}

    //angle in radians, as always
    Quaternion(Vec3<T> axis, T angle) {
      T half = angle/2;
      T s = sin(half);
      x = axis.x * s;
      y = axis.y * s;
      z = axis.z * s;
      w = cos(half);
    }

    Quaternion(Vec3<T> euler) {
      T c1 = cos(euler.z * 0.5);
      T c2 = cos(euler.y * 0.5);
      T c3 = cos(euler.x * 0.5);
      T s1 = sin(euler.z * 0.5);
      T s2 = sin(euler.y * 0.5);
      T s3 = sin(euler.x * 0.5);

      x = c1*c2*c3 + s1*s2*s3;
      y = c1*s2*c3 + s1*c2*s3;
      z = s1*c2*c3 - c1*s2*s3;
      w = c1*c2*s3 - s1*s2*c3;
    }

    T getAbs() const {
      return sqrt(sqr(x) + sqr(y) + sqr(z) + sqr(w));
    }

    void makeUnit() {
      *this /= getAbs();
    }
    
    Quaternion getUnit() const {
      return *this / getAbs();
    }

    Quaternion getConjugate() const {
      return Quaternion(-x, -y, -z, w);
    }

    Mat4<T> getMat() const {
      
      float x2 = x * x;
      float y2 = y * y;
      float z2 = z * z;
      float xy = x * y;
      float xz = x * z;
      float yz = y * z;
      float wx = w * x;
      float wy = w * y;
      float wz = w * z;
      
      /*T data[16] = {
	1 - 2 * (y2 + z2), 2 * (xy - wz), 2 * (xz + wy), 0,
	2 * (xy + wz), 1 - 2 * (x2 + z2), 2 * (yz - wx), 0,
	2 * (xz - wy), 2 * (yz + wx), 1 - 2 * (x2 + y2), 0,
	0, 0, 0, 1
	};*/
      T data[16] = {
	1 - 2 * (y2 + z2), 2 * (xy + wz), 2 * (xz - wy), 0,
	2 * (xy - wz), 1 - 2 * (x2 + z2), 2 * (yz + wx), 0,
	2 * (xz + wy), 2 * (yz - wx), 1 - 2 * (x2 + y2), 0,
	0, 0, 0, 1
      };
      return Mat4<T>(data);
    }

    String toString() {
      return to_string(w) + " + "
	+ to_string(x) + "i + "
	+ to_string(y) + "j + "
	+ to_string(z) + "k";
    }

    void operator=(const Quaternion b) {
      memcpy(this, &b, sizeof(Quaternion<T>));
    }

    Quaternion operator*(const T scal) const {
      return Quaternion(x*scal, y*scal, z*scal, w*scal);
    }
    Quaternion operator/(const T scal) const {
      return Quaternion(x/scal, y/scal, z/scal, w/scal);
    }
    void operator *=(const T scal) {
      *this = *this * scal;
    }
    void operator /=(const T scal) {
      *this = *this / scal;
    }

    Quaternion operator*(const Quaternion rq) const {
      	return Quaternion(w*rq.x + x*rq.w + y*rq.z - z*rq.y,
	                  w*rq.y + y*rq.w + z*rq.x - x*rq.z,
	                  w*rq.z + z*rq.w + x*rq.y - y*rq.x,
	                  w*rq.w - x*rq.x - y*rq.y - z*rq.z);
    }
    void operator *=(const Quaternion rq) {
      *this = rq * *this;
    }

    Vec3<T> operator*(const Vec3<T> vec) const {
      
      Vec3<T> vn = vec.getUnit();
      Quaternion vec_q(vn.x, vn.y, vn.z, 0);
      Quaternion res = vec_q * getConjugate();
      res *= *this;
      return Vec3<T>(res.x, res.y, res.z);
      
    }

  };

  typedef Quaternion<GLfloat> Quaternionf;

}
