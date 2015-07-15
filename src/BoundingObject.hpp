#pragma once

#include "VectorMath.hpp"
#include "BasicMesh.hpp"

NAMESPACE {

  struct Node;

  enum BoundingObjectType {
    BOUNDING_SPHERE,
    BOUNDING_OBB,
    BOUNDING_NONE
  };

  struct BoundingSphere  {

    Vec3f center;
    f32 radius;

    BoundingSphere(Array<BasicMeshData> data);
    f32 getVolume();
    f32 getInertia(f32 mass);
    void transform(Node* t);
    bool testIntersection(BoundingSphere b);
  };

  struct BoundingOBB {

    Vec3f center;
    Vec3f halves;
    Vec3f coord[3];

    BoundingOBB(Array<BasicMeshData> data);
    f32 getVolume();
    f32 getInertia(f32 mass);
    Vec3f getClosestPoint(Vec3f point);
    void transform(Node* t);
    bool testIntersection(BoundingOBB b);
    bool testIntersection(BoundingSphere b);
    
  };

  struct BoundingObject {
    
    BoundingObjectType type;
    union {
      BoundingSphere sphere;
      BoundingOBB obb;
    };

    BoundingObject(BoundingObjectType obj_type,
		   Array<BasicMeshData> data);
    f32 getVolume();
    f32 getInertia(f32 mass);
    void transform(Node* t);
  };

  struct Manifold {

    Vec3f normal;
    f32 penetration;

    //note that these assume the objects have already collided
    Manifold(BoundingSphere a, BoundingSphere b);
    Manifold(BoundingOBB a, BoundingOBB b);
    Manifold(BoundingSphere a, BoundingOBB b);
    Manifold(BoundingOBB a, BoundingSphere b);
    Manifold(BoundingObject a, BoundingObject b);
  };

  /*struct BoundingAABB : BoundingObject {
    
    Vec3f min;
    Vec3f max;

    BoundingAABB(Array<Vec3f> points);
    
  }*/

  bool testIntersection(BoundingObject a, BoundingObject b);

}
