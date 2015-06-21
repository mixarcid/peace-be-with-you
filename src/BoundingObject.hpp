#pragma once

#include "VectorMath.hpp"
#include "BasicMesh.hpp"

NAMESPACE {

  enum BoundingObjectType {
    BOUNDING_SPHERE
  };

  struct BoundingObject {
    
    BoundingObjectType type;

    BoundingObject(BoundingObjectType obj_type);
    
  };
  
  struct BoundingSphere : BoundingObject {

    Vec3f center;
    float radius;

    BoundingSphere(Array<BasicMeshData> data);
    
  };

  /*struct BoundingAABB : BoundingObject {
    
    Vec3f min;
    Vec3f max;

    BoundingAABB(Array<Vec3f> points);
    
  }*/

  bool testIntersection(BoundingObject* a, BoundingObject* b);

}
