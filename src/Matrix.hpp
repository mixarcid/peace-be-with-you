#pragma once

#include "Vector.hpp"

NAMESPACE {

  template <typename T, const u8 N>
    struct Mat {

    union {
      T data[N*N];
      Vec<T,N> cols[N];
    };

    inline Mat() {
      memset(this, 0, sizeof(Mat));
      for (u8 i=0; i<N; ++i) {
	cols[i].data[i] = 1;
      }
    }
    
    inline Mat(const T* initial_data) {
      memcpy(this, initial_data, sizeof(Mat));
    }

    inline Vec<T,N> row(const u8 index) const {
      Vec<T,N> ret;
      for (u8 i=0; i<N; ++i) {
	ret.data[i] = cols[i].data[index];
      }
      return ret;
    }

    inline Vec<T,N> col(const u8 index) const {
      return cols[index];
    }

    inline T& operator()(const u8 row,
		      const u8 col) {
      debugAssert(row < N && col < N,
		  "Matrix index out of bounds");
      return cols[col][row];
    }

    inline void operator=(Mat b) {
      memcpy(this, &b, sizeof(Mat));
    }

    inline Mat operator+(const Mat b) const {
      return Mat::add(*this, b);
    }

    inline Mat operator*(const T b) const {
      return Mat::mul(*this, b);
    }
    
    inline Vec<T,N> operator*(const Vec<T,N> b) const {
      return Mat::mul(*this, b);
    }

    inline Mat operator*(const Mat b) const {
      return Mat::mul(*this, b);
    }

    inline void operator*=(const Mat b) {
      *this = Mat::mul(*this, b);
    }

    static inline Mat add(const Mat a, const Mat b) {
      Mat ret;
      for (u8 i=0; i<N; ++i) {
	ret.cols[i] = a.cols[i] + b.cols[i];
      }
      return ret;
    }

    static inline Mat mul(const Mat m, const T s) {
      Mat ret;
      for (u8 i=0; i<N; ++i) {
	ret.data[i] = m.data[i]*s;
      }
      return ret;
    }

    static inline Mat div(const Mat m, const T s) {
      Mat ret;
      for (u8 i=0; i<N; ++i) {
	ret.data[i] = m.data[i]/s;
      }
      return ret;
    }

    static inline Vec<T,N> mul(const Mat m,
			       const Vec<T,N> v) {
      Vec<T,N> ret;
      for (u8 i=0; i<N; ++i) {
	ret.data[i] = Vec<T,N>::dot(v, m.row(i));
      }
      return ret;
    }

    static inline Vec<T,N> mulT(const Mat m,
				const Vec<T,N> v) {
      Vec<T,N> ret;
      for (u8 i=0; i<N; ++i) {
	ret.data[i] = Vec<T,N>::dot(v, m.col(i));
      }
      return ret;
    }

    static inline Mat mul(const Mat a, const Mat b) {
      Mat ret;
      for (u8 row=0; row<N; ++row) {
        for (u8 col=0; col<N; ++col) {
	  ret(row,col) =
	    Vec<T,N>::dot(a.row(row),b.col(col));
	}
      }
      return ret;
    }

    //multiply, treating the Matrix A as transposed
    static inline Mat mulT1(const Mat a, const Mat b) {
      Mat ret;
      for (u8 row=0; row<N; ++row) {
        for (u8 col=0; col<N; ++col) {
	  ret.cols[col].data[row] =
	    Vec<T,N>::dot(a.col(row),b.col(col));
	}
      }
      return ret;
    }

    //multiply, treating the Matrix B as transposed
    static inline Mat mulT2(const Mat a, const Mat b) {
      Mat ret;
      for (u8 row=0; row<N; ++row) {
        for (u8 col=0; col<N; ++col) {
	  ret.cols[col].data[row] =
	    Vec<T,N>::dot(a.row(row),b.row(col));
	}
      }
      return ret;
    }
    
      
  };

  template <typename T, const u8 N>
    inline String to_string(const Mat<T,N> m) {
    String ret = "\n";
    for (u8 row=0; row<N; ++row) {
      ret += "| ";
      for (u8 col=0; col<N; ++col) {
	ret += to_string(m(row,col)) + " ";
      }
      ret += "|\n";
    }
    return ret;
  }

  template <const u8 N>
    inline String to_string(const Mat<f32,N> m) {
    String ret = "\n";
    for (u8 row=0; row<N; ++row) {
      ret += "| ";
      for (u8 col=0; col<N; ++col) {
	ret += str::format("%7.3f ", m(row,col));
      }
      ret += "|\n";
    }
    return ret;
  }
  
  template <typename T>
    struct Mat3 : Mat<T,3> {

    using Mat<T,3>::Mat;
    inline Mat3() : Mat<T,3>::Mat() {}
    inline Mat3(Mat<T,3> init) {
      memcpy(this, &init, sizeof(Mat3));
    }
    
  };

  template <typename T>
    struct Mat4 : Mat<T,4> {

    using Mat<T,4>::Mat;
    inline Mat4() : Mat<T,4>::Mat() {}
    inline Mat4(Mat<T,4> init) {
      memcpy(this, &init, sizeof(Mat4));
    }

    inline Mat4(Mat3<T> m) {
      T d[16] = {m.data[0], m.data[1], m.data[2], 0,
		 m.data[3], m.data[4], m.data[5], 0,
		 m.data[6], m.data[7], m.data[8], 0,
		 0, 0, 0, 1};
      memcpy(this, d, sizeof(Mat4<T>));
    }

    static inline Mat4 scale(Vec3<T> scal) {
      T d[16] = {scal.x(), 0, 0, 0,
	     0, scal.y(), 0, 0,
	     0, 0, scal.z(), 0,
	     0, 0, 0, 1};
      return Mat4(d);
    }

    static inline Mat4 translate(Vec3<T> trans) {
      T d[16] = {1, 0, 0, 0,
	     0, 1, 0, 0,
	     0, 0, 1, 0,
	     trans.x(), trans.y(), trans.z(), 1};
      return Mat4(d);
    }

    static inline Mat4 scaleTrans(Vec3<T> scal,
				  Vec3<T> trans) {
      T d[16] = {scal.x(), 0, 0, 0,
		 0, scal.y(), 0, 0,
		 0, 0, scal.z(), 0,
		 trans.x(), trans.y(), trans.z(), 1};
      return Mat4(d);
    }

    static Mat4 perspective(T fovy, T aspect, T z_near, T z_far) {
      T sub = z_near - z_far;
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
      Vec3<T> f = (center - eye).normalized();
      Vec3<T> s = (Vec3<T>::cross(f, up)).normalized();
      Vec3<T> u = Vec3<T>::cross(s, f); 
      T data[16] = {
	s.x(), u.x(), -f.x(), 0,
	s.y(), u.y(), -f.y(), 0,
	s.z(), u.z(), -f.z(), 0,
	-Vec3<T>::dot(s, eye), -Vec3<T>::dot(u, eye),
	Vec3<T>::dot(f, eye), 1
      };
      return Mat4(data);
    }
    
  };

  typedef Mat3<f32> Mat3f;
  typedef Mat4<f32> Mat4f;
  
}
