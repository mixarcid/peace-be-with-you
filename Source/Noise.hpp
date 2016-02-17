#pragma once
#include "Vector.hpp"
#include "Function.hpp"

NAMESPACE {

  struct Noise {
    
    static f32 fractal(function<f32(Vec2f,i32)> noise_func,
		       Vec2f input, i32 num_octaves,
		       f32 lac = 2.0, f32 gain = 0.5);

  };
  
}
