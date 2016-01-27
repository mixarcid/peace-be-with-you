#pragma once

#include "Transform.hpp"
#include "BasicMesh.hpp"

NAMESPACE {

  enum BoundingObjectType {
    BOUNDING_SPHERE,
    BOUNDING_OBB,
    BOUNDING_GROUND,
    BOUNDING_NONE
  };

  struct BoundingAABB {

    Vec3f center;
    Vec3f halves;

    BoundingAABB(Vec3f _center, Vec3f _halves);
    f32 getVolume();
    f32 getInertia(f32 mass);
    bool someInBox(BoundingAABB box);
    bool allInBox(BoundingAABB box);
    void transform(Transform t);
  };

  struct BoundingSphere  {

    Vec3f center;
    f32 radius;

    BoundingSphere(Array<BasicMeshData> data);
    f32 getVolume();
    f32 getInertia(f32 mass);
    bool someInBox(BoundingAABB box);
    bool allInBox(BoundingAABB box);
    void transform(Transform t);
  };

  struct BoundingOBB {

    Vec3f center;
    Vec3f halves;
    Mat3f coord;

    BoundingOBB(Array<BasicMeshData> data);
    f32 getVolume();
    f32 getInertia(f32 mass);
    Vec3f getClosestPoint(Vec3f point);
    bool someInBox(BoundingAABB box);
    bool allInBox(BoundingAABB box);
    void transform(Transform t);
    
  };

  //only for terrain
  struct BoundingGround {

    typedef f32 (*GroundDataFunc) (Vec2f, Vec3f*);
    
    /*returns the height at the Vec2f xy point
      also changes the second, Vec3f argument
      to the normal at the point (if not NULL)
    */
    GroundDataFunc dataAtPoint;

    f32 getVolume();
    f32 getInertia(f32 mass);
    bool someInBox(BoundingAABB box);
    bool allInBox(BoundingAABB box);
    void transform(Transform t);
    
  };

  struct BoundingObject {
    
    BoundingObjectType type;
    union {
      BoundingSphere sphere;
      BoundingOBB obb;
      BoundingGround ground;
    };

    BoundingObject(BoundingObjectType _type = BOUNDING_NONE);
    BoundingObject(BoundingObjectType obj_type,
		   Array<BasicMeshData> data);
    
    f32 getVolume();
    f32 getInertia(f32 mass);
    
    //checks if object is partially contained within
    //box with center at center and with half length halves
    bool someInBox(BoundingAABB box);
    //check if object is fully within the same sort of box
    bool allInBox(BoundingAABB box);
    
    void transform(Transform t);
  };

}
