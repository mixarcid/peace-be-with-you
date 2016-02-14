#pragma once

#include "BoundingObject.hpp"

NAMESPACE {
  
  struct BoundingAABB : BoundingObjectBase<BOUNDING_AABB> {

    Vec3f center;
    Vec3f halves;

    BoundingAABB() {}
    BoundingAABB(Vec3f _center, Vec3f _halves);
    virtual f32 getVolume();
    virtual f32 getInertia(f32 mass);
    virtual void transform(Transform t);
    
  };

}
