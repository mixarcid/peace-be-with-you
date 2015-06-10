#pragma once
  
#include "Standard.hpp"
#include "Containers.hpp"
#include "Color.hpp"
#include "VectorMath.hpp"
#include "VertexObjects.hpp"
#include "Renderable.hpp"
#include "Texture.hpp"

NAMESPACE {

  struct RectData {
    Vec3f pos;
    Vec2f tex_coord;
  };

  struct TexturedRect : public RenderableEBO<RectData> {

    Texture tex;

    TexturedRect(const char* filename);
    virtual void render();
    
  };
}

/*NAMESPACE {

  struct RectData {
    Vec3f pos;
    Vec2f tex_coord;
  };

  struct TexturedRect {

    Array<RectData> data;
    Array<GLuint> elems;
    VBO vbo;
    EBO ebo;
    Texture tex;

    TexturedRect(const char* filename);
    virtual void render();

    static unsigned int vao;
  };
  }*/
