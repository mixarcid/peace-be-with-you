#pragma once
  
#include "Standard.h"
#include "Containers.h"
#include "Color.h"
#include "VectorMath.h"
#include "VertexObjects.h"
#include "Renderable.h"
#include "Texture.h"

NAMESPACE {

  struct RectData {
    Vec3f pos;
    Vec2f tex_coord;
  };

  struct TexturedRect : Renderable {

    Array<RectData> data;
    Array<GLuint> elems;
    VBO vbo;
    EBO ebo;
    Texture tex;

    TexturedRect(const char* filename);
    virtual void render();

    static unsigned int vao;
  };
}
