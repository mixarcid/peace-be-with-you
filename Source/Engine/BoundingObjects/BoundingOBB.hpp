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
    f32 getVolume();
    f32 getInertia(f32 mass);
    void transform(Transform t);
    Pointer<RenderableComp> getRenderable(Mat4f* model);
    Vec3f getCenter();
    
  };

  typedef BoundingOBB BoundingObb;
  
}
