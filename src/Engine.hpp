#pragma once

#include "Graphics.hpp"
#include "Input.hpp"
#include "Time.hpp"
#include "Physics.hpp"
#include "Assets.hpp"
#include "GameObject.hpp"

NAMESPACE {

  PEACE_DEFINE_BITFLAGS(EngineFlags, 8,
			ENGINE_RUNNING = 0x01,
			ENGINE_GRAPHICS_INIT = 0x02,
			ENGINE_ASSETS_LOADED = 0x04);

  struct Engine {

    EngineFlags flags;
    SystemManager system_manager;
    GLFWwindow* window;
    Graphics* graphics;
    Physics* physics;
    HandledArray<GameObject*> update_objects;
    HashMap<String, GameObject*> named_objects;

    Time cur_time, prev_time;
    f32 dt;

    Engine();
    GameObjectHandle addGameObject(GameObject* object);
    void removeGameObject(GameObjectHandle h);
    void loop();
    void begin();
    ~Engine();
    
  };

}
