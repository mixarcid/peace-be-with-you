#pragma once

#include "Math.hpp"

NAMESPACE {

  template <typename T, const u8 N>
    struct Vec {
    
    T data[N];

    inline Vec() {
      memset(this, 0, sizeof(Vec));
    }

    inline T norm() const {
      T ret = 0;
      for (u8 i=0; i<N; ++i) {
	ret += sqr((*this).data[i]);
      }
      return sqrt(ret);
    }

    inline Vec normalized() const {
      return (*this)/norm();
    }

    inline void normalize() {
      *this = (*this).normalized();
    }

    inline T& operator[](const u8 index) {
      debugAssert(index < N,
		  "Vec index must be less than %u", N);
      return data[index];
    }

    inline void operator=(const Vec b) {
      memcpy(this, &b, sizeof(Vec));
    }

    inline Vec operator+(const Vec b) const {
      return add(*this, b);
    }

    inline Vec operator-(const Vec b) const {
      return sub(*this, b);
    }

    inline Vec operator-() const {
      return neg(*this);
    }

    inline void operator+=(const Vec b) {
      *this = add(*this, b);
    }

    inline void operator-=(const Vec b) {
      *this = sub(*this, b);
    }

    inline Vec operator*(const T b) const {
      return mul(*this, b);
    }

    inline Vec operator/(const T b) const {
      return div(*this, b);
    }

    inline void operator*=(const T b) {
      *this = mul(*this, b);
    }

    inline void operator/=(const T b) {
      *this = div(*this, b);
    }

    inline bool is_zero() const {
      for (u8 i=0; i<N; ++i) {
	if (!(floatEqual(data[i],T(0)))) return false;
      }
      return true;
    }

    static inline Vec mul(const Vec a, const T b) {
      Vec ret;
      for (u8 i=0; i<N; ++i) {
	ret.data[i] = a.data[i]*b;
      }
      return ret;
    }

    static inline Vec div(const Vec a, const T b) {
      Vec ret;
      for (u8 i=0; i<N; ++i) {
	ret.data[i] = a.data[i]/b;
      }
      return ret;
    }

    static inline Vec add(const Vec a, const Vec b) {
      Vec ret;
      for (u8 i=0; i<N; ++i) {
	ret.data[i] = a.data[i] + b.data[i];
      }
      return ret;
    }
    
    static inline Vec sub(const Vec a, const Vec b) {
      Vec ret;
      for (u8 i=0; i<N; ++i) {
	ret.data[i] = a.data[i] - b.data[i];
      }
      return ret;
    }

    static inline Vec neg(const Vec a) {
      Vec ret;
      for (u8 i=0; i<N; ++i) {
	ret.data[i] = -a.data[i];
      }
      return ret;
    }

    static inline T dot(const Vec a, const Vec b) {
      T ret = 0;
      for (u8 i=0; i<N; ++i) {
	ret += a.data[i]*b.data[i];
      }
      return ret;
    }

    static inline Vec lerp(const Vec a,
			   const Vec b,
			   const T h) {
      debugAssert(h >= 0.0 && h <= 1.0,
		  "H must be between 0 and 1 for Vec lerp");
      return a*(1-h) + b*h;
    }
  };

  template <typename T, const u8 N>
    inline String to_string(const Vec<T,N> v) {
    String ret = "<";
    for (u8 i=0; i<N-1; ++i) {
      ret += to_string(v.data[i]) + ", ";
    }
    ret += to_string(v.data[N-1]) + ">";
    return ret;
  }

  template <const u8 N>
    inline String to_string(const Vec<f32,N> v) {
    String ret = "<";
    for (u8 i=0; i<N-1; ++i) {
      ret += str::format("%1.3f, ", v.data[i]);
    }
    ret += str::format("%1.3f>", v.data[N-1]);
    return ret;
  }

  template <typename T>
    struct Vec2 : Vec<T,2> {

    using Vec<T,2>::Vec;

    inline Vec2(Vec<T,2> init) {
      memcpy(this, &init, sizeof(Vec2));
    }

    inline Vec2() : Vec<T,2>::Vec() {}

    inline Vec2(T x, T y) {
      Vec<T,2>::data[0] = x;
      Vec<T,2>::data[1] = y;
    }

    inline T& x() {
      return Vec<T,2>::data[0];
    }
    
    inline T& y() {
      return Vec<T,2>::data[1];
    }

    //smae as above, but for const Vecs
    inline T cx() const {
      return Vec<T,2>::data[0];
    }
    
    inline T cy() const {
      return Vec<T,2>::data[1];
    }
    
  };

  template <typename T>
    struct Vec3 : Vec<T,3> {

    using Vec<T,3>::Vec;

    inline Vec3(Vec<T,3> init) {
      memcpy(this, &init, sizeof(Vec3));
    }

    inline Vec3() : Vec<T,3>::Vec() {}

    inline Vec3(T x, T y, T z) {
      Vec<T,3>::data[0] = x;
      Vec<T,3>::data[1] = y;
      Vec<T,3>::data[2] = z;
    }

    inline T& x() {
      return Vec<T,3>::data[0];
    }
    
    inline T& y() {
      return Vec<T,3>::data[1];
    }
    
    inline T& z() {
      return Vec<T,3>::data[2];
    }

    inline T& r() {
      return Vec<T,3>::data[0];
    }
    
    inline T& g() {
      return Vec<T,3>::data[1];
    }
    
    inline T& b() {
      return Vec<T,3>::data[2];
    }

    inline T cx() const {
      return Vec<T,3>::data[0];
    }
    
    inline T cy() const {
      return Vec<T,3>::data[1];
    }
    
    inline T cz() const {
      return Vec<T,3>::data[2];
    }

    inline T cr() const {
      return Vec<T,3>::data[0];
    }
    
    inline T cg() const {
      return Vec<T,3>::data[1];
    }
    
    inline T cb() const {
      return Vec<T,3>::data[2];
    }

    inline Vec2<T> xy() const {
      return Vec2<T>(cx(),cy());
    }

    static inline Vec3 cross(Vec3 v1, Vec3 v2) {
      return Vec3((v1.y()*v2.z()) - (v1.z()*v2.y()),
		  (v1.z()*v2.x()) - (v1.x()*v2.z()),
		  (v1.x()*v2.y()) - (v1.y()*v2.x()));
    }
    
  };

  template <typename T>
    struct Vec4 : Vec<T,4> {

    using Vec<T,4>::Vec;

    inline Vec4(Vec<T,4> init) {
      memcpy(this, &init, sizeof(Vec4));
    }

    inline Vec4() : Vec<T,4>::Vec() {}

    inline Vec4(T x, T y, T z, T w) {
      Vec<T,4>::data[0] = x;
      Vec<T,4>::data[1] = y;
      Vec<T,4>::data[2] = z;
      Vec<T,4>::data[3] = w;
    }

    inline T& x() {
      return Vec<T,4>::data[0];
    }
    
    inline T& y() {
      return Vec<T,4>::data[1];
    }
    
    inline T& z() {
      return Vec<T,4>::data[2];
    }

    inline T& w() {
      return Vec<T,4>::data[3];
    }

    inline T& r() {
      return Vec<T,4>::data[0];
    }
    
    inline T& g() {
      return Vec<T,4>::data[1];
    }
    
    inline T& b() {
      return Vec<T,4>::data[2];
    }

    inline T& a() {
      return Vec<T,4>::data[3];
    }

    inline T cx() const {
      return Vec<T,4>::data[0];
    }
    
    inline T cy() const {
      return Vec<T,4>::data[1];
    }
    
    inline T cz() const {
      return Vec<T,4>::data[2];
    }

    inline T cw() const {
      return Vec<T,4>::data[3];
    }

    inline T cr() const {
      return Vec<T,4>::data[0];
    }
    
    inline T cg() const {
      return Vec<T,4>::data[1];
    }
    
    inline T cb() const {
      return Vec<T,4>::data[2];
    }

    inline T ca() const {
      return Vec<T,4>::data[3];
    }
    
  };


  typedef Vec2<f32> Vec2f;
  typedef Vec3<f32> Vec3f;
  typedef Vec4<f32> Vec4f;
  typedef Vec2<u32> Vec2u;
  typedef Vec3<u32> Vec3u;
  typedef Vec4<u32> Vec4u;
  typedef Vec2<i16> Vec2s;
  typedef Vec3<i16> Vec3s;
  typedef Vec4<i16> Vec4s;
  
}
