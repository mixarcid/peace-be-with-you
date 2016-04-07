#pragma once
#include "Vector.hpp"
#include "Function.hpp"
#include "Perlin.hpp"
#include "Matrix.hpp"

NAMESPACE {

  struct Noise {
    
    static f32 fractal(function<f32(Vec2f,i32)> noise_func,
		       Vec2f input, i32 num_octaves,
		       f32 lac = 2.0, f32 gain = 0.5);

  };

  struct FrankNoise {

    Perlin perlin;
    Mat2d rot;

    FrankNoise();
    f32 getValue(Vec2f point, i32 index = 0);
    //equivalent to 0.5 + 0.5*(getValue(point, index))
    f32 getPositive(Vec2f point, i32 index = 0);
    
  };
  
}
