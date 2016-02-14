#include "BoundingGround.hpp"

NAMESPACE {

  BoundingGround::BoundingGround(GroundDataFunc func)
    : dataAtPoint(func) {}
  
  f32 BoundingGround::getVolume() {
    PEACE_UNIMPLIMENTED(0);
  }
  
  f32 BoundingGround::getInertia(f32 mass) {
    PEACE_UNIMPLIMENTED(0);
  }

  void BoundingGround::transform(Transform t) {
    PEACE_UNIMPLIMENTED();
  }
 
}
