#pragma once

#include "Node.hpp"

NAMESPACE {

  struct Graphics3d {

    Array<Node*> nodes;
    Shader shade;
    Camera* cam;

    void addNode(Node* node);
    vois setCamera(Camera* camera);
    void setShader(String name);
    void render(GLFWWindow* window);
  }

}
