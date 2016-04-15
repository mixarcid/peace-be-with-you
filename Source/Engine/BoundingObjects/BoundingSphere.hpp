#pragma once

#include "BoundingObject.hpp"
#include "BasicMesh.hpp"

NAMESPACE {

  struct BoundingSphere : BoundingObjectBase<BoundingObject::SPHERE> {

    Vec3f center;
    f32 radius;

    BoundingSphere() {}
    BoundingSphere(Vec3f _center, f32 _radius)
      : center(_center), radius(_radius) {}
    BoundingSphere(Array<BasicMeshData>& data);
    f32 getVolume();
    f32 getInertia(f32 mass);
    void transform(Transform t);
    Vec3f getCenter();
    Pointer<RenderableComp> getRenderable(Mat4f* model) {
      return Pointer<RenderableComp>::null;
    }
    
  };

}
