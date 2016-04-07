#include "Perlin.hpp"
#include "Time.hpp"

NAMESPACE {

  f64 lerp(f64 a, f64 b, f64 t) {
    /*debugAssert(t>0 && t<1,
      "t must be between 0 and 1 for lerp");*/
    return a + t*(b-a);
  }
  
  f64 perlinFade(f64 t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
  }

  f64 perlinGrad(i16 hash, f64 x, f64 y) {
   switch(hash & 0xf) {
   case 0x0: return  x + y;
   case 0x1: return -x + y;
   case 0x2: return  x - y;
   case 0x3: return -x - y;
   case 0x4: return  x + y;
   case 0x5: return -x + y;
   case 0x6: return  x - y;
   case 0x7: return -x - y;
   case 0x8: return  x + y;
   case 0x9: return -x + y;
   case 0xa: return  x - y;
   case 0xb: return -x - y;
   case 0xc: return  x + y;
   case 0xd: return -x + y;
   case 0xe: return  x - y;
   case 0xf: return -x - y;
   default: return 0;
    }
  }

  const i16 SEED_SIZE = 256;
  const i16 SEED_MAX_VAL = SEED_SIZE - 1;
  
  Perlin::Perlin() {

    Time t;
    t.makeCurrent();
    srand(t.getMilliseconds());
    
    for (i16 i=0; i<SEED_SIZE; ++i) {
      seed[i] = i;
    }

    //Fisher-Yates Shuffle
    for (i16 i=0; i<SEED_SIZE-2; ++i) {
      i16 j = rand() % (SEED_SIZE - i);
      i16 tmp = seed[i];
      seed[i] = seed[i+j];
      seed[i+j] = tmp;
    }

    for (i16 i=0; i<SEED_SIZE; ++i) {
      seed[i+SEED_SIZE] = seed[i];
    }
    
  }
  
  f64 Perlin::getValue(Vec2d input) {

    debugAssert(input.x() > 0 &&
		input.y() > 0,
		"Perlin::getValue has strange discontinuties at x = 0 "
		"and y = 0; only use positive numbers");
    
    i16 xi = (i16) input.x() & SEED_MAX_VAL;
    i16 yi = (i16) input.y() & SEED_MAX_VAL;
    f64 xf = input.x() - floor(input.x());
    f64 yf = input.y() - floor(input.y());
    f64 u = perlinFade(xf);
    f64 v = perlinFade(yf);

    i16 aa, ab, ba, bb;
    aa = seed[seed[xi]+yi];
    ab = seed[seed[xi]+yi+1];
    ba = seed[seed[xi+1]+yi];
    bb = seed[seed[xi+1]+yi+1];

    f64 x1, x2;
    x1 = lerp(perlinGrad(aa, xf, yf), 
	      perlinGrad(ba, xf-1, yf),
	      u);
    x2 = lerp(perlinGrad(ab, xf, yf-1),
	      perlinGrad (bb, xf-1, yf-1),
	      u);
    return lerp(x1, x2, v); 
  }

}
