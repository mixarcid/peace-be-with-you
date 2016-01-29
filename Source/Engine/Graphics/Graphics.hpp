#pragma once

#include "Light.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "GUI.hpp"
#include "Containers.hpp"

NAMESPACE {

  struct Engine;

  struct Graphics {

    Array<GUINode> gui_nodes;
    Array<DirLight> dir_lights;
    Engine* engine;
    GLFWwindow* window;
    Camera cam;
    Shader shade;
    Vec4f background_color;
    f32 ambient;

    Graphics(Engine* _engine);
    ~Graphics();
    
    void init(const String vert, const String frag);
    
    template <typename... Args>
    void emplaceDirLight(Args... args) {
      debugAssert(dir_lights.size() <
		  Shader::MAX_DIR_LIGHTS,
		  "Trying to add too many directional "
		  "lights to scene");
      dir_lights.emplace_back(args...);
    }
    
    void render(GLFWwindow* window);

    static Vec2f window_size;
  };

}
