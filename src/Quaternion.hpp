#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"

NAMESPACE {

  template <typename T>
    struct Quaternion {

    T data[4];

    inline Quaternion(T qx, T qy, T qz, T qw) {
      data[0] = qx;
      data[1] = qy;
      data[2] = qz;
      data[3] = qw;
    }

    inline Quaternion() {
      data[0] = 0;
      data[1] = 0;
      data[2] = 0;
      data[3] = 1;
    }

    //angle in radians, as always
    inline Quaternion(Vec3<T> axis, T angle) {
      T half = angle/2;
      T s = sin(half);
      data[0] = axis.x * s;
      data[1] = axis.y * s;
      data[2] = axis.z * s;
      data[3] = cos(half);
    }

    inline Quaternion(T euler_x, T euler_y, T euler_z) {
      T c1 = cos(euler_z * 0.5);
      T c2 = cos(euler_y * 0.5);
      T c3 = cos(euler_x * 0.5);
      T s1 = sin(euler_z * 0.5);
      T s2 = sin(euler_y * 0.5);
      T s3 = sin(euler_x * 0.5);

      data[0] = c1*c2*s3 - s1*s2*c3;
      data[1] = c1*s2*c3 + s1*c2*s3;
      data[2] = s1*c2*c3 - c1*s2*s3;
      data[3] = c1*c2*c3 + s1*s2*s3;
    }

    inline T x() const {
      return data[0];
    }

    inline T y() const {
      return data[1];
    }

    inline T z() const {
      return data[2];
    }

    inline T w() const {
      return data[3];
    }

    inline T norm() const {
      return sqrt(sqr(x()) + sqr(y()) + sqr(z()) + sqr(w()));
    }

    inline void normalize() {
      *this /= norm();
    }
    
    inline Quaternion normalized() const {
      return *this / norm();
    }

    inline Quaternion conjugate() const {
      return Quaternion(-x(), -y(), -z(), w());
    }

    inline Mat3<T> mat3() const {
      
      f32 x2 = x() * x();
      f32 y2 = y() * y();
      f32 z2 = z() * z();
      f32 xy = x() * y();
      f32 xz = x() * z();
      f32 yz = y() * z();
      f32 wx = w() * x();
      f32 wy = w() * y();
      f32 wz = w() * z();
      
      T data[16] = {
	1 - 2 * (y2 + z2), 2 * (xy + wz), 2 * (xz - wy),
	2 * (xy - wz), 1 - 2 * (x2 + z2), 2 * (yz + wx),
	2 * (xz + wy), 2 * (yz - wx), 1 - 2 * (x2 + y2)
      };
      return Mat3<T>(data);
    }

    inline Mat4<T> mat4() const {
      return Mat4<T>(mat3());
    }

    inline T operator[](const u8 index) {
      return data[index];
    }

    inline void operator=(const Quaternion b) {
      memcpy(this, &b, sizeof(Quaternion<T>));
    }

    inline Quaternion operator+(const Quaternion b) {
      return add(*this, b);
    }
    inline Quaternion operator-(const Quaternion b) {
      return sub(*this, b);
    }
    inline Quaternion operator*(const T scal) const {
      return mul(*this, scal);
    }
    inline Quaternion operator/(const T scal) const {
      return div(*this, scal);
    }
    inline void operator *=(const T scal) {
      *this = *this * scal;
    }
    inline void operator /=(const T scal) {
      *this = *this / scal;
    }

    inline Quaternion operator*(const Quaternion rq) const {
      return mul(*this, rq);
    }
    inline void operator *=(const Quaternion rq) {
      *this = rq * *this;
    }

    inline Vec3<T> operator*(const Vec3<T> vec) const {
      return rotate(*this, vec);
    }

    static inline Quaternion add(const Quaternion a, const Quaternion b) {
      return Quaternion(a.x() + b.x(),
			a.y() + b.y(),
			a.z() + b.z(),
			a.w() + b.w());
    }

    static inline Quaternion sub(const Quaternion a, const Quaternion b) {
      return Quaternion(a.x() - b.x(),
			a.y() - b.y(),
			a.z() - b.z(),
			a.w() - b.w());
    }

    static inline Quaternion mul(const Quaternion a, const T scal) {
      return Quaternion(a.x()*scal, a.y()*scal, a.z()*scal, a.w()*scal);
    }
    
    static inline Quaternion div(const Quaternion a, const T scal) {
      return Quaternion(a.x()/scal, a.y()/scal, a.z()/scal, a.w()/scal);
    }

    static inline Quaternion mul(const Quaternion a, const  Quaternion b) {
      return Quaternion(a.w()*b.x() + a.x()*b.w() + a.y()*b.z() - a.z()*b.y(),
			a.w()*b.y() + a.y()*b.w() + a.z()*b.x() - a.x()*b.z(),
			a.w()*b.z() + a.z()*b.w() + a.x()*b.y() - a.y()*b.x(),
			a.w()*b.w() - a.x()*b.x() - a.y()*b.y() - a.z()*b.z());
    }

    static inline Vec3<T> rotate(const Quaternion q, const Vec3<T> vec) {
      
      Vec3<T> vn = vec.normalized();
      Quaternion vec_q(vn.x(), vn.y(), vn.z(), 0);
      Quaternion res = vec_q * q.conjugate();
      res *= q;
      return Vec3<T>(res.x(), res.y(), res.z());
      
    }

    static inline Quaternion lerp(Quaternion a, Quaternion b, T h) {
      debugAssert(h >= 0.0 && h <= 1.0,
		  "H must be between 0 and 1 for Quaternion lerp");
      return a*(1-h) + b*h;
    }

    static inline Quaternion slerp(Quaternion a, Quaternion b, T h) {
      
      debugAssert(h > 0.0 && h < 1.0,
		  "H must be between 0 and 1 for Quaternion slerp");
      T cos_half_angle = a.x()*b.x() + a.y()*b.y() + a.z()*b.z() + a.w()*b.w();
      
      if (abs(cos_half_angle) >= 1.0f) {
	return a;
      }
      
      T half_angle = acos(cos_half_angle);
      T sin_half_angle = sqrt(1 - sqr(cos_half_angle));
      T r1 = sin((1 - h) * half_angle)/sin_half_angle;
      T r2 = sin(h * half_angle)/sin_half_angle;
      
      return Quaternion(a.x()*r1 + b.x()*r2,
			a.y()*r1 + b.y()*r2,
			a.z()*r1 + b.z()*r2,
			a.w()*r1 + b.w()*r2);
    }

  };
  
  template <typename T>
    inline String to_string(Quaternion<T> q) {
    return to_string(q.w()) + " + "
      + to_string(q.x()) + "i + "
      + to_string(q.y()) + "j + "
      + to_string(q.z()) + "k";
  }

  template <>
    inline String to_string(Quaternion<f32> q) {
    return str::format("%1.3f + %1.3fi + %1.3fj + %1.3fk", q.w(),q.x(),q.y(),q.z());
  }


  typedef Quaternion<f32> Quaternionf;

}
