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
 
  FrankNoise::FrankNoise() {
    f64 cost = rand()/(f64)RAND_MAX;
    f64 sint = sqrt(1 - cost*cost);
    f64 arr[4] = {cost, sint, -sint, cost};
    rot = Mat2d(arr);
  }

  const static Vec2d PERLIN_OFFSET(1000000.0,2000000.0);
  f32 FrankNoise::getValue(Vec2f point, i32 index) {
    Mat2d r = rot;
    for (i32 i=0; i<index; ++i) {
      r *= rot;
    }
    return perlin.getValue(rot*Vec2d(point) + PERLIN_OFFSET*(index+1));
  }
  
    f32 FrankNoise::getPositive(Vec2f point, i32 index) {
      return 0.5 + 0.5*getValue(point, index);
  }
}
