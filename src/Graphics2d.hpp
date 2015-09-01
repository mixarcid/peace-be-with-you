#pragma once

#include "Node2d.hpp"

NAMESPACE {

  struct Graphics2d {

    Array<Renderable2d*> renderables;

    void addRenderable(Renderable2d* rend);
    void render(GLFWwindow* window);
    
  };

}
