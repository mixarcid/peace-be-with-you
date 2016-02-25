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
    
    Texture tex;
    BoundingSphere b_sphere;
    BoundingOBB b_obb;
    
    StaticMesh(Texture texture,
	       ShaderFlags flags = SHADER_FLAGS);
    
    void init(Array<BasicMeshData>& data,
	      Array<u32>& elements);
    BoundingObject* getTightBoundingObject();
    BoundingObject* getLooseBoundingObject();
    virtual void render(RenderContext c);

    const static ShaderFlags SHADER_FLAGS
    = (SHADER_3D | SHADER_USE_NORMAL | SHADER_USE_TEXTURE);

  };
  
}
