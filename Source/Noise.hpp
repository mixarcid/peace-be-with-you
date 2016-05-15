#pragma once
#include "Vector.hpp"
#include "Function.hpp"
#include "Perlin.hpp"
#include "Matrix.hpp"

NAMESPACE {

  struct Noise {
    
    static f64 fractal(function<f64(Vec2d,i32)> noise_func,
		       Vec2d input, i32 num_octaves,
		       f64 lac = 2.0, f64 gain = 0.5);

  };

  struct FrankNoise {

    Perlin perlin;
    Mat2d rot;

    FrankNoise();
    f64 getValue(Vec2d point, i32 index = 0);
    //equivalent to 0.5 + 0.5*(getValue(point, index))
    f64 getPositive(Vec2d point, i32 index = 0);
    
  };
  
}
