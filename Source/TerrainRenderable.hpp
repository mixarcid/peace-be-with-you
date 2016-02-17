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

    TerrainRenderable();
    void init(Array<BasicMeshData>& data);
    virtual void render(RenderContext c);

  };

}
