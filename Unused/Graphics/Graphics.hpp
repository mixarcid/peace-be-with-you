#pragma once

#include "Light.hpp"
#include "Shader.hpp"
#include "HandledArray.hpp"

NAMESPACE {

  struct Camera;
  struct Node;
  struct GUINode;

  struct Graphics {

    f32 ambient;
    Vec4f background_color;
    HandledArray<Node*> nodes;
    HandledArray<GUINode*> nodes_2d;
    HandledArray<DirLight*> dir_lights;
    Camera* cam;
    Shader shade;

    Graphics(Vec4f _background_color = Vec4f(0,0,0,1));
    void init(const String shader_name);
    ArrayHandle addNode(Node* node);
    ArrayHandle addGUINode(GUINode* node);
    ArrayHandle addDirLight(DirLight* light);
    void removeNode(ArrayHandle h);
    void removeGUINode(ArrayHandle h);
    void removeDirLight(ArrayHandle h);
    void setAmbient(f32 ambient_light);
    void setCamera(Camera* camera);
    //void setShader(String name);
    void render(GLFWwindow* window, f32 dt);

    static Vec2f window_size;
  };

}