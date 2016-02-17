#include "Noise.hpp"

NAMESPACE {

  f32 Noise::fractal(function<f32(Vec2f,i32)> noise_func,
		     Vec2f input, i32 num_octaves,
		     f32 lac, f32 gain) {
    f32 f = 1.0f;
    f32 a = 1.0f;
    f32 ret = 0;
    for (i32 i=0; i<num_octaves; ++i) {
      ret += a*(noise_func(input*f,i));
      f *= lac;
      a *= gain;
    }
    return ret;
  }

}
