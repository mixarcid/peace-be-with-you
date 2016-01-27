#pragma once

#include "Time.hpp"
#include "GameObject.hpp"
#include "Graphics.hpp"

NAMESPACE {

  PEACE_DEFINE_BITFLAGS(EngineFlags, 8,
			ENGINE_NO_FLAGS = 0x00,
			ENGINE_RUNNING = 0x01,
			ENGINE_GRAPHICS_INIT = 0x02,
			ENGINE_ASSETS_LOADED = 0x04);
  
  struct Engine {

    Array<GameObject> game_objects;
    SystemManager system_manager;
    Graphics graphics;
    GLFWwindow* window;
    Time cur_time;
    Time prev_time;
    f32 dt;
    EngineFlags flags;

    Engine();
    ~Engine();

    void init();

    //Game Objects MUST be created using only this method
    template <typename T, typename... Args>
    void emplaceObject(Args... args) {
      game_objects.emplace_back<T>(this, args...);
    }

    void removeObject(GameObject* object);
    
    void loop();
    void begin();
    
  };

}
