#include <string.h>
#include <math.h>
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

}
