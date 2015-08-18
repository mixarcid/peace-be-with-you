#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"
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
  };

  struct BoundingOBB {

    Vec3f center;
    Vec3f halves;
    Mat3f coord;

    BoundingOBB(Array<BasicMeshData> data);
    f32 getVolume();
    f32 getInertia(f32 mass);
    Vec3f getClosestPoint(Vec3f point);
    void transform(Node* t);
    
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

}
