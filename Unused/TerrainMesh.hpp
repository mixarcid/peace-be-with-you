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

  struct TerrainMesh : Renderable {

    VBO vbo;
    VAO vao;
    EBO* ebo;
    Texture* tex;
    Array<BasicMeshData> data;
    
    TerrainMesh(EBO* _ebo,
		Texture* texture);
    
    void init();
    virtual void render(RenderContext c);

    const static ShaderFlags FLAGS;
    
  };
  
}
