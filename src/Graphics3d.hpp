#pragma once

#include "Shader.hpp"
#include "Light.hpp"

NAMESPACE {

  struct Camera;
  struct Node;

  struct Graphics3d {

    f32 ambient;
    Array<Node*> nodes;
    Array<DirLight*> dir_lights;
    Shader shade;
    Camera* cam;

    Graphics3d(String shader_name);
    
    void addNode(Node* node);
    void addDirLight(DirLight* light);
    void setAmbient(f32 ambient_light);
    void setCamera(Camera* camera);
    //void setShader(String name);
    void render(GLFWwindow* window, f32 dt);
  };

}
