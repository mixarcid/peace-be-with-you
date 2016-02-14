#pragma once

#include "BoundingObject.hpp"
#include "BoundingSphere.hpp"
#include "BoundingAABB.hpp"
#include "BoundingOBB.hpp"
#include "BoundingGround.hpp"

NAMESPACE {
  
  struct BoundingObjectUnion {
    
    union {
      BoundingSphere sphere;
      BoundingAABB aabb;
      BoundingOBB obb;
      BoundingGround ground;
      BoundingObject object;
    };

    BoundingObjectUnion() {}

    BoundingObject* getBoundingObject() {
      return (BoundingObject*) this;
    }
    
  };
}
