#pragma once

#include "Standard.h"
#include "Renderable.h"
#include "VectorMath.h"
#include "String.h"
#include "Containers.h"
#include "VertexObjects.h"

NAMESPACE {

  struct StaticMeshData {

    Vec3f pos;
    Vec3f norm;
    Vec2f tex_coord;

  };

  struct StaticMesh : Renderable {

    Array<StaticMeshData> data;
    VBO vbo;

    StaticMesh(String filename);
    virtual void render();

    static unsigned int vao;
    
  };
  
}
