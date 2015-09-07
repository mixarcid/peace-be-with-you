#pragma once

#include "Light.hpp"
#include "Shader.hpp"

NAMESPACE {

  struct Camera;
  struct Node;
  struct GUINode;

  struct Graphics {

    f32 ambient;
    Array<Node*> nodes;
    Array<GUINode*> nodes_2d;
    Array<DirLight*> dir_lights;
    Camera* cam;
    Shader shade;

    Graphics(String shader_name);
    void addNode(Node* node);
    void addGUINode(GUINode* node);
    void addDirLight(DirLight* light);
    void setAmbient(f32 ambient_light);
    void setCamera(Camera* camera);
    //void setShader(String name);
    void render(GLFWwindow* window, f32 dt);

    static Vec2f window_size;
  };

}
