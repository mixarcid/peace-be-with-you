#pragma once

#include "Shader.hpp"
#include "Containers.hpp"
#include "GLObject.hpp"

NAMESPACE {

  struct ScreenQuad {

    VBO vbo;
    VAO vao;

    ScreenQuad() {}
    void init(ShaderVar var);
    void render();

    static Array<Vec2f> data;
  };

}
