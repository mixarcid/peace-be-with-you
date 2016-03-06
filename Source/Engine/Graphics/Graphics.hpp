#pragma once

#include "Light.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "GUI.hpp"
#include "Containers.hpp"
#include "Renderer.hpp"
#include "GameObject.hpp"
#include "BoundingFrustum.hpp"

NAMESPACE {

  struct Engine;

  PEACE_DEFINE_BITFLAGS
    (GraphicsFlags, 8,
     GRAPHICS_NO_FLAGS = 0x00,
     GRAPHICS_RENDER_BOUNDING_TIGHT = 0x01,
     GRAPHICS_RENDER_BOUNDING_LOOSE = 0x02);
  
  struct Graphics {

    Array<GUINode> gui_nodes;
    Array<DirLight> dir_lights;
    Engine* engine;
    GLFWwindow* window;
    Pointer<Camera> cam;
    Renderer renderer;
    Vec2i win_size;
    f32 ambient;
    GraphicsFlags flags;

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
    void render();

  };

}
