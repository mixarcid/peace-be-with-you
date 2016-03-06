#pragma once

#include "BoundingObject.hpp"
#include "BasicMesh.hpp"

NAMESPACE {

  struct BoundingSphere : BoundingObjectBase<BoundingObject::SPHERE> {

    Vec3f center;
    f32 radius;

    BoundingSphere() {}
    BoundingSphere(Array<BasicMeshData>& data);
    virtual f32 getVolume();
    virtual f32 getInertia(f32 mass);
    virtual void transform(Transform t);
    
  };

}
