#pragma once

#include "Time.hpp"
#include "GameObject.hpp"
#include "Graphics.hpp"
#include "Physics.hpp"
#include "Function.hpp"
#include "QuadTree.hpp"
#include "Audio.hpp"

NAMESPACE {

  struct BoundingObject;

  PEACE_DEFINE_BITFLAGS(EngineFlags, 8,
			ENGINE_NO_FLAGS = 0x00,
			ENGINE_RUNNING = 0x01,
			ENGINE_GRAPHICS_INIT = 0x02,
			ENGINE_ASSETS_LOADED = 0x04,
			//whether to use the first static container buffer
			//or the second
			ENGINE_CONTAINER_FIRST = 0x08,
			ENGINE_CONTAINER_SWAP = 0x10);
  
  struct Engine {

    template <typename Obj, typename Comp>
    using ObjectCallBack = function<bool(Pointer<Obj>&, Pointer<Comp>&)>;

    typedef void (*CallBack) ();

    Array<StaticObject> static_objects;
    Array<DynamicObject> dynamic_objects;
    Mutex static_arr_mutex;

    Array<CallBack> scene_callbacks;
    Array<CallBack> synchronized_callbacks;
    Mutex synchronized_mutex;
    
    Grid dynamic_container;
    QuadTree static_containers[2];
    Array<Pointer<DynamicObject>> unregistered_dynamic_objects;
    
    SystemManager system_manager;
    Graphics graphics;
    Physics physics;
    Audio audio;
    GLFWwindow* window;
    Time cur_time;
    Time prev_time;
    u64 num_seconds;
    f32 dt;

    Vec2f prev_cam_pos;
    f32 cam_move_radius;

    Mutex flag_mutex;
    EngineFlags flags;

    static Engine* engine;

    Engine();
    ~Engine() throw();

    void _container_update();

    static void init();
    static void terminate();

    QuadTree* getStaticContainer(bool cur_container);
    void swapContainers();

    static void registerMove(Pointer<DynamicObject>& obj);

    static void initCamPos(bool cur_container = true);

    static void loop();
    static void begin();
    static void stop();

    //GameObjects MUST be created using only these methods
    
    template <typename T, typename... Args>
    static Pointer<T> emplaceStaticNoRegister(Args... args) {
      engine->static_arr_mutex.lock();
      Pointer<StaticObject> ret(engine->static_objects.emplace_back<T>(args...));
      engine->static_arr_mutex.unlock();
      return Pointer<T>((Pointer<T>&)ret);
    }

    template <typename T>
    static void registerStatic(Pointer<T>& obj, bool cur_container = true) {
      engine->getStaticContainer(cur_container)->insert((Pointer<StaticObject>&)obj);
    }

    template <typename T, typename... Args>
    static Pointer<T> emplaceStatic(Args... args) {
      engine->static_arr_mutex.lock();
      Pointer<StaticObject> ret(engine->static_objects.emplace_back<T>(args...));
      registerStatic(ret);
      engine->static_arr_mutex.unlock();
      return Pointer<T>((Pointer<T>&)ret);
    }
    
    template <typename T, typename... Args>
    static Pointer<T> emplaceDynamic(Args... args) {
      Pointer<DynamicObject> ret(engine->dynamic_objects.emplace_back<T>(args...));
      engine->dynamic_container.insert(ret);
      return Pointer<T>((Pointer<T>&)ret);
    }

    template <typename T, typename... Args>
    static Pointer<T> emplaceDynamicNoRegister(Args... args) {
      Pointer<DynamicObject> ret(engine->dynamic_objects.emplace_back<T>(args...));
      engine->unregistered_dynamic_objects.push_back(ret);
      return Pointer<T>((Pointer<T>&)ret);
    }
    
    template <typename T>
    static void removeDynamic(Pointer<T>& handle) {
      engine->dynamic_container.remove
	((Pointer<DynamicObject>&)handle);
      engine->dynamic_objects.removeAndReplace
        ((Pointer<DynamicObject>&) handle);
    }
    
    template <typename T>
    static void removeStatic(Pointer<T> handle) {
      engine->static_arr_mutex.lock();
      engine->static_objects.removeAndReplace
        ((Pointer<StaticObject>&) handle);
      engine->static_arr_mutex.unlock();
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
      engine->getStaticContainer(true)->traverse
	(bound, [callback](Pointer<StaticObject>& b) -> bool {
	  Pointer<T>& comp = b->getComponent<T>();
	  if (comp && !callback(b, comp)) {
	    return false;
	  }
	  return true;
	});
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
	(obj,
	 [callback](Pointer<DynamicObject>& b) -> bool {
	  Pointer<T>& comp = b->getComponent<T>();
	  if (comp && !callback(b, comp)) {
	    return false;
	  }
	  return true;
	});
    }
    
  };

}
