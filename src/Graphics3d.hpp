#pragma once

#include "Light.hpp"

NAMESPACE {

  struct Camera;
  struct Node;

  struct Graphics3d {

    f32 ambient;
    Array<Node*> nodes;
    Array<DirLight*> dir_lights;
    Camera* cam;

    void addNode(Node* node);
    void addDirLight(DirLight* light);
    void setAmbient(f32 ambient_light);
    void setCamera(Camera* camera);
    //void setShader(String name);
    void render(GLFWwindow* window, f32 dt);
  };

}
