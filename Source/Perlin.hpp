#pragma once
#include "Vector.hpp"

NAMESPACE {

  /*Thanks a lot to Adrian Biagioli for his excellent
    Perlin noise tutorial:
    http://flafla2.github.io/2014/08/09/perlinnoise.html
  */
  
  struct Perlin {

    u16 seed[512];

    Perlin();
    f32 getValue(Vec2f input);
    
  };

}
