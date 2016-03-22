#pragma once

#include "BoundingObject.hpp"
#include "BoundingSphere.hpp"
#include "BoundingAABB.hpp"
#include "BoundingOBB.hpp"
#include "BoundingGround.hpp"
#include "BoundingFrustum.hpp"
#include "BoundingAABB2D.hpp"

NAMESPACE {
  
  struct BoundingObjectUnion {
    
    union {
      FOR(entry in getEnumEntries("BoundingObjectType")[:-3]);
      Bounding$(className(entry)) obj_$(entry);
      END_FOR;
      BoundingObject object;
    };

    BoundingObjectUnion();
    ~BoundingObjectUnion() {}
    BoundingObject* getBoundingObject();
    void set(BoundingObject* obj);
    
  };
}
