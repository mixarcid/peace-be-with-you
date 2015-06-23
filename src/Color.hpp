#pragma once

#include "Standard.hpp"
#include "GL.hpp"

NAMESPACE {

  template <typename T>
    struct Color4 {

    T r;
    T g;
    T b;
    T a;

    Color4(T red, T green, T blue, T alpha = 1.0) {
      r = red;
      g = green;
      b = blue;
      a = alpha;
    }
    
  };

  typedef Color4<f32> Color4f;
  
}
