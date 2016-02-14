#pragma once

#include "Standard.hpp"
#include "RenderableComp.hpp"
#include "Vector.hpp"
#include "Containers.hpp"
#include "Texture.hpp"
#include "BasicMesh.hpp"
#include "BoundingSphere.hpp"
#include "BoundingOBB.hpp"

NAMESPACE {

  struct StaticMesh : RenderableReg {
    
    Array<BasicMeshData> data;
    Array<u32> elements;
    Texture tex;
    BoundingSphere b_sphere;
    BoundingOBB b_obb;
    
    StaticMesh(Texture texture,
	       ShaderFlags _shader_flags
	       = SHADER_NO_FLAGS);
    
    void init();
    void init(EBO ebo);
    BoundingObject* getTightBoundingObject();
    BoundingObject* getLooseBoundingObject();
    virtual void render(RenderContext c);

  };
  
}
