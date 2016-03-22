#pragma once

#include "BoundingObject.hpp"

NAMESPACE {

  struct BoundingGround : BoundingObjectBase<BoundingObject::GROUND> {

    //typedef f32 (*GroundDataFunc) (Vec2f, Vec3f*);
    typedef function<f32(Vec2f, Vec3f*)> GroundDataFunc;
    
    /*returns the height at the Vec2f xy point
      also changes the second, Vec3f argument
      to the normal at the point (if not NULL)
    */
    GroundDataFunc dataAtPoint;

    BoundingGround(GroundDataFunc func);
    f32 getVolume();
    f32 getInertia(f32 mass);
    
  };


}
