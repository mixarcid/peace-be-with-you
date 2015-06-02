#pragma once

#include <GLFW/glfw3.h>
#include <GL/glc.h>
#include "Standard.h"

NAMESPACE {

  template <typename T>
  struct Color {
    
    T r;
    T g;
    T b;
    T a;

    Color(T red, T green, T blue, T alpha = 1) {
      r = red;
      g = green;
      b = blue;
      a = alpha;
    }
  };

  typedef Color<float> Colorf;
  typedef Color<double> Colord;
}
