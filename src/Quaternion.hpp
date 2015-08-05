#pragma once

#include "VectorMath.hpp"

NAMESPACE {

  template <typename T>
    struct Quaternion {

    T x;
    T y;
    T z;
    T w;

    Quaternion(T qx, T qy, T qz, T qw)
      : x(qx), y(qy), z(qz), w(qw) {}

    Quaternion()
      : x(0), y(0), z(0), w(1) {}

    //angle in radians, as always
    Quaternion(Vec3<T> axis, T angle) {
      T half = angle/2;
      T s = sin(half);
      x = axis.x * s;
      y = axis.y * s;
      z = axis.z * s;
      w = cos(half);
    }

    Quaternion(T euler_x, T euler_y, T euler_z) {
      T c1 = cos(euler_z * 0.5);
      T c2 = cos(euler_y * 0.5);
      T c3 = cos(euler_x * 0.5);
      T s1 = sin(euler_z * 0.5);
      T s2 = sin(euler_y * 0.5);
      T s3 = sin(euler_x * 0.5);

      w = c1*c2*c3 + s1*s2*s3;
      y = c1*s2*c3 + s1*c2*s3;
      z = s1*c2*c3 - c1*s2*s3;
      x = c1*c2*s3 - s1*s2*c3;

      this->makeUnit();
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

    Mat4<T> getMat4() const {
      
      f32 x2 = x * x;
      f32 y2 = y * y;
      f32 z2 = z * z;
      f32 xy = x * y;
      f32 xz = x * z;
      f32 yz = y * z;
      f32 wx = w * x;
      f32 wy = w * y;
      f32 wz = w * z;
      
      T data[16] = {
	1 - 2 * (y2 + z2), 2 * (xy + wz), 2 * (xz - wy), 0,
	2 * (xy - wz), 1 - 2 * (x2 + z2), 2 * (yz + wx), 0,
	2 * (xz + wy), 2 * (yz - wx), 1 - 2 * (x2 + y2), 0,
	0, 0, 0, 1
      };
      return Mat4<T>(data);
    }

    Mat3<T> getMat3() const {
      
      f32 x2 = x * x;
      f32 y2 = y * y;
      f32 z2 = z * z;
      f32 xy = x * y;
      f32 xz = x * z;
      f32 yz = y * z;
      f32 wx = w * x;
      f32 wy = w * y;
      f32 wz = w * z;
      
      T data[16] = {
	1 - 2 * (y2 + z2), 2 * (xy + wz), 2 * (xz - wy),
	2 * (xy - wz), 1 - 2 * (x2 + z2), 2 * (yz + wx),
	2 * (xz + wy), 2 * (yz - wx), 1 - 2 * (x2 + y2)
      };
      return Mat3<T>(data);
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

    Quaternion operator+(const Quaternion b) {
      return Quaternion(x + b.x,
			y + b.y,
			z + b.z,
			w + b.w);
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

    static Quaternion lerp(Quaternion a, Quaternion b, T h) {
      debugAssert(h > 0.0 && h < 1.0,
		  "H must be between 0 and 1 for Quaternion lerp");
      return a*(1-h) + b*h;
    }

    static Quaternion slerp(Quaternion a, Quaternion b, T h) {
      
      debugAssert(h > 0.0 && h < 1.0,
		  "H must be between 0 and 1 for Quaternion slerp");
      T cos_half_angle = a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
      
      if (abs(cos_half_angle) >= 1.0f) {
	return a;
      }
      
      T half_angle = acos(cos_half_angle);
      T sin_half_angle = sqrt(1 - sqr(cos_half_angle));
      T r1 = sin((1 - h) * half_angle)/sin_half_angle;
      T r2 = sin(h * half_angle)/sin_half_angle;
      
      return Quaternion(a.x*r1 + b.x*r2,
			a.y*r1 + b.y*r2,
			a.z*r1 + b.z*r2,
			a.w*r1 + b.w*r2);
    }

  };

  typedef Quaternion<f32> Quaternionf;

}
