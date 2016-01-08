#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"
#include "BasicMesh.hpp"

NAMESPACE {

  struct Node;

  enum BoundingObjectType {
    BOUNDING_SPHERE,
    BOUNDING_OBB,
    BOUNDING_GROUND,
    BOUNDING_NONE
  };

  struct BoundingSphere  {

    Vec3f center;
    f32 radius;

    BoundingSphere(Array<BasicMeshData> data);
    f32 getVolume();
    f32 getInertia(f32 mass);
    bool someInBox(Vec3f center, Vec3f halves);
    bool allInBox(Vec3f center, Vec3f halves);
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
    bool someInBox(Vec3f center, Vec3f halves);
    bool allInBox(Vec3f center, Vec3f halves);
    void transform(Node* t);
    
  };

  struct BoundingAABB {

    Vec3f center;
    Vec3f halves;

    f32 getVolume();
    f32 getInertia(f32 mass);
    bool someInBox(Vec3f center, Vec3f halves);
    bool allInBox(Vec3f center, Vec3f halves);
    void transform(Node* t);
  };

  //only for terrain
  struct BoundingGround {
    /*returns the height at the Vec2f xy point
      also changes the second, Vec3f argument
      to the normal at the point (if not NULL)
    */
    function<f32(Vec2f,Vec3f*)> dataAtPoint;

    f32 getVolume();
    f32 getInertia(f32 mass);
    bool someInBox(Vec3f center, Vec3f halves);
    bool allInBox(Vec3f center, Vec3f halves);
    void transform(Node* t);
    
  };

  struct BoundingObject {
    
    BoundingObjectType type;
    union {
      BoundingSphere sphere;
      BoundingOBB obb;
      BoundingGround ground;
    };

    BoundingObject(BoundingObjectType _type = BOUNDING_OBB);
    BoundingObject(BoundingObjectType obj_type,
		   Array<BasicMeshData> data);
    BoundingObject(const BoundingObject& obj);
    ~BoundingObject() {}
    f32 getVolume();
    f32 getInertia(f32 mass);
    
    //checks if object is partially contained within
    //box with center at center and with half length halves
    bool someInBox(Vec3f center, Vec3f halves);
    //check if object is fully within the same sort of box
    bool allInBox(Vec3f center, Vec3f halves);
    
    void transform(Node* t);
  };

}
