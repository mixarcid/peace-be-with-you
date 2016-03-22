#pragma once

#include "BoundingObject.hpp"
#include "BasicMesh.hpp"

NAMESPACE {

  struct BoundingSphere : BoundingObjectBase<BoundingObject::SPHERE> {

    Vec3f center;
    f32 radius;

    BoundingSphere() {}
    BoundingSphere(Array<BasicMeshData>& data);
    f32 getVolume();
    f32 getInertia(f32 mass);
    void transform(Transform t);
    Vec3f getCenter();
    void render(RenderContext c) {}
    
  };

}
