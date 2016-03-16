#pragma once

#include "BoundingObject.hpp"

NAMESPACE {
  
  struct BoundingAABB2D : BoundingObjectBase<BoundingObject::AABB2D> {

    Vec2f center;
    Vec2f halves;

    BoundingAABB2D() {}
    BoundingAABB2D(Vec2f _center, Vec2f _halves);
    Vec2f getClosestPoint(Vec2f point);
    bool pointInside(Vec2f point);
    virtual Vec3f getCenter();
    
  };

}
