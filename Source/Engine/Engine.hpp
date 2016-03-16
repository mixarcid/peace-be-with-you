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

    template <typename Obj, typename Comp>
    using ObjectCallBack = function<bool(Pointer<Obj>&, Pointer<Comp>&)>;

    Array<StaticObject> static_objects;
    Array<DynamicObject> dynamic_objects;
    Grid dynamic_container;
    
    SystemManager system_manager;
    Graphics graphics;
    Physics physics;
    GLFWwindow* window;
    Time cur_time;
    Time prev_time;
    f32 dt;
    EngineFlags flags;

    static Engine* engine;

    Engine();
    ~Engine();

    static void init();
    static void terminate();

    //GameObjects MUST be created using only these methods
    template <typename T, typename... Args>
    static Pointer<T> emplaceStatic(Args... args) {
      return Pointer<T>(engine->static_objects.emplace_back<T>(args...));
    }
    template <typename T, typename... Args>
    static Pointer<T> emplaceDynamic(Args... args) {
      Pointer<DynamicObject> ret(engine->dynamic_objects.emplace_back<T>(args...));
      ret->handle = engine->dynamic_container.insert
	((Pointer<GameObject>&) ret);
      return Pointer<T>((Pointer<T>&)ret);
    }

    template <typename T>
    static void removeStatic(Pointer<T>& handle) {
      engine->static_objects.removeAndReplace
	((Pointer<StaticObject>&)handle);
    }
    template <typename T>
    static void removeDynamic(Pointer<T>& handle) {
      engine->dynamic_container.remove
	((Pointer<DynamicObject>&)handle, handle->handle);
      engine->dynamic_objects.removeAndReplace
        ((Pointer<DynamicObject>&) handle);
    }

    /*
      note that these methods do NOT check bound for intersecting
      with the object before calling callback
      calling these functions with bound equal to NULL will result
      in the traversal of all objects
      the traversal will immediately stop if the callback returns false
    */
    template <typename T>
    static void traverseStatic
    (BoundingObject* bound,
     ObjectCallBack<StaticObject,T> callback) {
      for (StaticObject& obj : engine->static_objects) {
	Pointer<StaticObject> obj_p(&obj);
	Pointer<T>& comp = obj.getComponent<T>();
	if (comp && !callback(obj_p, comp)) {
	  break;
	}
      }
    }
    
    template <typename T>
    static void traverseDynamic
    (BoundingObject* bound,
     ObjectCallBack<DynamicObject,T> callback) {
      for (DynamicObject& obj : engine->dynamic_objects) {
	Pointer<DynamicObject> obj_p(&obj);
	Pointer<T>& comp = obj.getComponent<T>();
	if (comp && !callback(obj_p, comp)) {
	  break;
	}
      }
    }

    template <typename T>
    static void traverseNeighbors
    (Pointer<DynamicObject>& obj,
     ObjectCallBack<DynamicObject,T> callback) {
      engine->dynamic_container.traverseNeighbors
	((Pointer<GameObject>&) obj,
	 obj->handle,
	 [callback](Pointer<GameObject>& b) -> bool {
	  Pointer<T>& comp = b->getComponent<T>();
	  if (comp && !callback((Pointer<DynamicObject>&)b, comp)) {
	    return false;
	  }
	  return true;
	});
    }

    static void registerMove(Pointer<DynamicObject>& obj);
    
    static void loop();
    static void begin();
    
  };

}
