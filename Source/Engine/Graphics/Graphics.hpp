#pragma once

#include "Light.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "GUI.hpp"
#include "Containers.hpp"
#include "Renderer.hpp"
#include "GameObject.hpp"

NAMESPACE {

  struct Engine;
  
  struct Graphics {

    Array<GUINode> gui_nodes;
    Array<DirLight> dir_lights;
    Engine* engine;
    GLFWwindow* window;
    Camera cam;
    Renderer renderer;
    Vec2i win_size;
    f32 ambient;

    Graphics(Engine* _engine);
    
    void init(GLFWwindow* _window);
    
    template <typename... Args>
    void emplaceDirLight(Args... args) {
      debugAssert(dir_lights.size() <
		  Shader::MAX_DIR_LIGHTS,
		  "Trying to add too many directional "
		  "lights to scene");
      dir_lights.emplace_back(args...);
    }
    
    void renderFunc(ComponentPair<RenderableComp> obj,
		    RenderContext c,
		    Mat4f model);
    void render();

  };

}
