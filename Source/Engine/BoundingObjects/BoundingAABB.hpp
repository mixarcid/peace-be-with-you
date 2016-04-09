#pragma once

#include "BoundingObject.hpp"

NAMESPACE {
  
  struct BoundingAABB : BoundingObjectBase<BoundingObject::AABB> {

    Vec3f center;
    Vec3f halves;

    BoundingAABB() {}
    BoundingAABB(Vec3f _center, Vec3f _halves);
    Vec3f getCenter();
    f32 getVolume();
    f32 getInertia(f32 mass);
    //current algorithm doesn't hadnle rotations
    void transform(Transform t);
    Pointer<RenderableComp> getRenderable(Mat4f* model);
    
  };

  typedef BoundingAABB BoundingAabb;

}
