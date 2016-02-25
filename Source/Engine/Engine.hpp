#pragma once

#include "Time.hpp"
#include "GameObject.hpp"
#include "Graphics.hpp"
#include "Physics.hpp"
#include "Function.hpp"

NAMESPACE {

  struct BoundingObject;

  PEACE_DEFINE_BITFLAGS(EngineFlags, 8,
			ENGINE_NO_FLAGS = 0x00,
			ENGINE_RUNNING = 0x01,
			ENGINE_GRAPHICS_INIT = 0x02,
			ENGINE_ASSETS_LOADED = 0x04);
  
  struct Engine {

    Array<GameObject> static_objects;
    Array<GameObject> dynamic_objects;
    SystemManager system_manager;
    Graphics graphics;
    Physics physics;
    GLFWwindow* window;
    Time cur_time;
    Time prev_time;
    f32 dt;
    EngineFlags flags;

    Engine();
    ~Engine();

    void init();

    //Game Objects MUST be created using only these methods
    template <typename T, typename... Args>
    Pointer<T> emplaceStatic(Args... args) {
      return Pointer<T>(static_objects.emplace_back<T>(this, args...));
    }
    template <typename T, typename... Args>
    Pointer<T> emplaceDynamic(Args... args) {
      return Pointer<T>(dynamic_objects.emplace_back<T>(this, args...));
    }

    template <typename T>
    void removeStatic(Pointer<T> handle) {
      static_objects.removeAndReplace(Pointer<GameObject>(handle));
    }
    template <typename T>
    void removeDynamic(Pointer<T> handle) {
      dynamic_objects.removeAndReplace(Pointer<GameObject>(handle));
    }

    /*
      note that these methods do NOT check bound for intersecting
      with the object before calling callback
      calling these functions with bound equal to NULL will result
      in the traversal of all objects
    */
    template <typename T>
    void traverseStatic(BoundingObject* bound,
			function<void(ComponentPair<T>)> callback) {
      for (GameObject& obj : static_objects) {
	Pointer<T> comp = obj.getComponent<T>();
	if (comp) {
	  callback(ComponentPair<T>(&obj, comp));
	}
      }
    }
    
    template <typename T>
    void traverseDynamic(BoundingObject* bound,
			 function<void(ComponentPair<T>)> callback) {
      for (GameObject& obj : dynamic_objects) {
	Pointer<T> comp = obj.getComponent<T>();
	if (comp) {
	  callback(ComponentPair<T>(&obj, comp));
	}
      }
    }
    void loop();
    void begin();
    
  };

}
