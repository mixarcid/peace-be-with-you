#pragma once

#include "Shader.hpp"

NAMESPACE {

  struct Camera;
  struct Node;

  struct Graphics3d {

    Array<Node*> nodes;
    Shader shade;
    Camera* cam;

    void addNode(Node* node);
    void setCamera(Camera* camera);
    void setShader(String name);
    void render(GLFWwindow* window);
  };

}
