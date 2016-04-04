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
    f32 cost = rand()/(f32)RAND_MAX;
    f32 sint = sqrt(1 - cost*cost);
    f32 arr[4] = {cost, sint, -sint, cost};
    rot = Mat2f(arr);
  }

  const static Vec2f PERLIN_OFFSET(1000.0f,2000.0f);
  f32 FrankNoise::getValue(Vec2f point) {
    return perlin.getValue(rot*point + PERLIN_OFFSET);
  }
  
  f32 FrankNoise::getPositive(Vec2f point) {
    return 0.5 + 0.5*getValue(point);
  }
}
