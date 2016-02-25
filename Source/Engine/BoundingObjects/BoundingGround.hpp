#pragma once

#include "BoundingObject.hpp"

NAMESPACE {

  struct BoundingGround : BoundingObjectBase<BoundingObject::GROUND> {

    typedef f32 (*GroundDataFunc) (Vec2f, Vec3f*);
    
    /*returns the height at the Vec2f xy point
      also changes the second, Vec3f argument
      to the normal at the point (if not NULL)
    */
    GroundDataFunc dataAtPoint;

    BoundingGround(GroundDataFunc func);
    virtual f32 getVolume();
    virtual f32 getInertia(f32 mass);
    virtual BoundingObject* transform(TransformBasic t);
    
  };


}
