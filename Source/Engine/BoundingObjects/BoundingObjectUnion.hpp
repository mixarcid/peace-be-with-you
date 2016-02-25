#pragma once

#include "BoundingObject.hpp"
#include "BoundingSphere.hpp"
#include "BoundingAABB.hpp"
#include "BoundingOBB.hpp"
#include "BoundingGround.hpp"


NAMESPACE {
  
  struct BoundingObjectUnion {

    //This code is hideous
    typedef BoundingOBB BoundingObb;
    typedef BoundingAABB BoundingAabb;
    
    union {
      FOR(entry in getEnumEntries("BoundingObjectType")[:-2]);
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
