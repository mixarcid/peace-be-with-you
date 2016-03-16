#pragma once

#include "BoundingObject.hpp"
#include "BasicMesh.hpp"

NAMESPACE {

  struct BoundingOBB : BoundingObjectBase<BoundingObject::OBB> {

    Vec3f center;
    Vec3f halves;
    Mat3f coord;

    BoundingOBB() {}
    BoundingOBB(Array<BasicMeshData>& data);
    Vec3f getClosestPoint(Vec3f point);
    virtual f32 getVolume();
    virtual f32 getInertia(f32 mass);
    virtual void transform(Transform t);
    virtual void render(RenderContext c);
    virtual Vec3f getCenter();
    
  };
  
}
