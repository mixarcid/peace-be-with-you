#include <string.h>
#include <math.h>
#include <float.h>
#include "Standard.hpp"

NAMESPACE {

  constexpr f64 degreesToRadians(f64 deg) {
    return deg*(M_PI/(f64)180);
  }

  template<typename T>
    inline T sqr(T x) {
    return x*x;
  }

  template<typename T>
    inline T min(T a, T b) {
    return a > b ? b : a;
  }

  template<typename T>
    inline T max(T a, T b) {
    return a > b ? a : b;
  }

  template<typename T>
    inline T abs(T num) {
    return num > 0 ? num : -num;
  }
  
  template <typename T>
    inline bool floatEqual(T a, T b) {
    return abs(a - b) < T(0.000001);
  }

}
