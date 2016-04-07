#pragma once

#include "Math.hpp"

NAMESPACE {

  template <typename T, const u8 N>
    struct Vec {
    
    T data[N];

    inline Vec() {
      memset(this, 0, sizeof(Vec));
    }

    template <typename U>
    inline Vec(Vec<U, N> b) {
      for (u8 i=0; i<N; ++i) {
	data[i] = b.data[i];
      }
    }

    template <typename U>
    inline Vec(Vec<U, N-1> vec, T val) {
      for (u8 i=0; i<N-1; ++i) {
	data[i] = vec.data[i];
      }
      data[N-1] = val;
    }

    inline T normSquared() const {
      T ret = 0;
      for (u8 i=0; i<N; ++i) {
	ret += sqr((*this).data[i]);
      }
      return ret;
    }

    inline T norm() const {
      return sqrt(normSquared());
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

    template<typename U>
    inline void operator=(const Vec<U, N> b) {
      for (u8 n=0; n<N; ++n) {
	data[n] = b.data[n];
      }
    }

    template<typename U>
    inline Vec operator+(const Vec<U, N> b) const {
      return add(*this, b);
    }

    template<typename U>
    inline Vec operator-(const Vec<U, N> b) const {
      return sub(*this, b);
    }

    inline Vec operator-() const {
      return neg(*this);
    }

    template<typename U>
    inline void operator+=(const Vec<U, N> b) {
      *this = add(*this, b);
    }

    template<typename U>
    inline void operator-=(const Vec<U, N> b) {
      *this = sub(*this, b);
    }

    template<typename U>
    inline Vec operator*(const U b) const {
      return mul(*this, b);
    }
    
    template<typename U>
    inline Vec operator/(const U b) const {
      return div(*this, b);
    }

    template<typename U>
    inline void operator*=(const U b) {
      *this = mul(*this, b);
    }

    template<typename U>
    inline void operator/=(const U b) {
      *this = div(*this, b);
    }

    inline bool is_zero() const {
      for (u8 i=0; i<N; ++i) {
	if (!(floatEqual(data[i],T(0)))) return false;
      }
      return true;
    }

    template<typename U>
    static inline Vec mul(const Vec a, const U b) {
      Vec ret;
      for (u8 i=0; i<N; ++i) {
	ret.data[i] = a.data[i]*b;
      }
      return ret;
    }

    template<typename U>
    static inline Vec div(const Vec a, const U b) {
      Vec ret;
      for (u8 i=0; i<N; ++i) {
	ret.data[i] = a.data[i]/b;
      }
      return ret;
    }

    template <typename U>
    static inline Vec add(const Vec a, const Vec<U, N> b) {
      Vec ret;
      for (u8 i=0; i<N; ++i) {
	ret.data[i] = a.data[i] + b.data[i];
      }
      return ret;
    }

    template <typename U>
    static inline Vec sub(const Vec a, const Vec<U, N> b) {
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

    template <typename U>
    static inline T dot(const Vec a, const Vec<U, N> b) {
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

  template<typename T, typename U, u8 N>
    inline bool operator==(const Vec<T, N> a,
			   const Vec<U, N> b) {
    for (u8 n=0; n<N; ++n) {
      if (a.data[n] != b.data[n]) return false;
    }
    return true;
  }

  template<typename T, typename U, u8 N>
    inline bool operator!=(const Vec<T, N> a,
			   const Vec<U, N> b) {
    for (u8 n=0; n<N; ++n) {
      if (a.data[n] != b.data[n]) return true;
    }
    return false;
  }


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
    
    inline operator Vec<T,2>() const {
      return *((Vec<T,2>)this);
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

    inline operator Vec<T,3>() const {
      return *((Vec<T,3>)this);
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

    inline operator Vec<T,4>() const {
      return *((Vec<T,4>)this);
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

    inline Vec2<T> xy() const {
      return Vec2<T>(cx(),cy());
    }

    inline Vec3<T> xyz() const {
      return Vec3<T>(cx(),cy(),cz());
    }
    
  };


  typedef Vec2<f32> Vec2f;
  typedef Vec3<f32> Vec3f;
  typedef Vec4<f32> Vec4f;

  typedef Vec2<f64> Vec2d;
  typedef Vec3<f64> Vec3d;
  typedef Vec4<f64> Vec4d;

  typedef Vec2<i32> Vec2i;
  typedef Vec3<i32> Vec3i;
  typedef Vec4<i32> Vec4i;
  
  typedef Vec2<u32> Vec2u;
  typedef Vec3<u32> Vec3u;
  typedef Vec4<u32> Vec4u;
  
  typedef Vec2<i16> Vec2s;
  typedef Vec3<i16> Vec3s;
  typedef Vec4<i16> Vec4s;

  typedef Vec2<u16> Vec2us;
  typedef Vec3<u16> Vec3us;
  typedef Vec4<u16> Vec4us;

  typedef Vec2<i8> Vec2b;
  typedef Vec3<i8> Vec3b;
  typedef Vec4<i8> Vec4b;

  typedef Vec2<u8> Vec2ub;
  typedef Vec3<u8> Vec3ub;
  typedef Vec4<u8> Vec4ub;  
  
}
