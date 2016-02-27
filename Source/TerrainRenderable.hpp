#pragma once
#include "RenderableComp.hpp"
#include "BasicMesh.hpp"
#include "Containers.hpp"
#include "Texture.hpp"

NAMESPACE {

  struct TerrainRenderable : RenderableComp {

    VAO vao;
    VBO vbo;
    Texture tex;
    Array<BasicMeshData> data;
    
    TerrainRenderable();
    void init();
    virtual void render(RenderContext c);
    
    const static ShaderFlags SHADER_FLAGS
    = SHADER_3D | SHADER_USE_NORMAL | SHADER_USE_TEXTURE;
    
  };

}
