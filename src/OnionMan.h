#pragma once
  
#include "Standard.h"
#include "Containers.h"
#include "Color.h"
#include "VectorMath.h"
#include "VertexObjects.h"
#include "Renderable.h"

NAMESPACE {

  struct OnionData {
    Vec3f pos;
    Color4f col;
  };

  struct OnionMan : Renderable {

    Array<OnionData> data;
    VAO vao;
    VBO vbo;

    OnionMan();
    virtual void render();
  };
  
}
