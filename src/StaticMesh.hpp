#pragma once

#include "Standard.hpp"
#include "Renderable.hpp"
#include "Vector.hpp"
#include "Containers.hpp"
#include "String.hpp"
#include "Texture.hpp"
#include "BasicMesh.hpp"
#include "BoundingObject.hpp"

NAMESPACE {

  struct StaticMesh : RenderableReg {
    
    Array<BasicMeshData> data;
    Array<u32> elements;
    Texture* tex;
    BoundingObject b_sphere;
    BoundingObject b_obb;
    
    StaticMesh(Array<BasicMeshData> mesh_data,
	       Array<u32> mesh_elems, Texture* texture);
    void init();
    virtual BoundingObject getBoundingObject();
    virtual BoundingObject getPhysicalBoundingObject();
    virtual BoundingObject getPrimaryBoundingObject();
    virtual void render(RenderContext c);
    
  };
  
}
