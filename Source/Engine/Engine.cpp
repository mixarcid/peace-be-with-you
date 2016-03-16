#include "Engine.hpp"
#include "Input.hpp"
#include "Assets.hpp"
#include "BoundingObject.hpp"

NAMESPACE {

  Engine* Engine::engine;

  Engine::Engine()
    : dynamic_container(20.0f),
    graphics(this),
    physics(this),
    dt(0),
    flags(ENGINE_NO_FLAGS) {
      //dynamic_objects.reserve(200);
    }

  Engine::~Engine() {
    if (flags * ENGINE_ASSETS_LOADED) {
      freeAllAssets();
    }
    if (flags & ENGINE_GRAPHICS_INIT) {
      gl::terminate();
    }
    Log::terminate();
  }

  void Engine::init() {
    engine = new Engine();
    Log::init(NULL);
    engine->system_manager.addSystem(Log::getSystem());
    engine->system_manager.start();

    gl::init();

    engine->window = gl::createWindow(Vec2s(700,700), "Peace be with You");
    Input::init(engine->window);
    
    const unsigned char* version = glGetString(GL_VERSION);
    fatalAssert(version != NULL,
		"Cannot determine OpenGL version");
    Log::message("Your OpenGL version is %s", version);
    
    engine->graphics.init(engine->window);

    engine->flags |= ENGINE_GRAPHICS_INIT;

    loadAllAssets();
    engine->flags |= ENGINE_ASSETS_LOADED;
    
  }

  void Engine::terminate() {
    delete engine;
  }

  void Engine::registerMove(Pointer<DynamicObject>& obj) {
    obj->handle = engine->dynamic_container.update
      ((Pointer<GameObject>&)obj, obj->handle);
  }
  
  void Engine::loop() {

    if (glfwWindowShouldClose(engine->window)) {
      engine->flags &= ~ENGINE_RUNNING;
      return;
    }
    
    engine->cur_time.makeCurrent();
    engine->dt = (f32)
      (engine->cur_time.getMilliseconds()
       - engine->prev_time.getMilliseconds())/1000;
    engine->physics.update();
    engine->graphics.render();
    gl::checkError();

    glfwPollEvents();

    //FPS calculation
    static Time last_second = engine->prev_time;
    static u32 num_frames = 0;
    static f32 fps = 0;

    ++num_frames;

    if (engine->cur_time.getMilliseconds() -
        last_second.getMilliseconds() > 1000) {
      fps = num_frames;
      num_frames = 0;
      last_second = engine->cur_time;
    }
    //Log::message("FPS: %f", fps);
    engine->prev_time = engine->cur_time;

    /*Log::message("Dynamic objects:");
    for (auto& elem : engine->dynamic_container.objects) {
      Log::message(to_string(elem.first));
      for (auto& obj : elem.second) {
	Log::message("  %p, %p", obj.data, obj.it);
      }
    }

      Log::message("------");
    for (auto& obj : engine->dynamic_objects) {
      Log::message(to_string(obj.pointers.size()) + ", "
		   + to_string(obj.handle.neighbors));
		   }*/
    
  }

  void Engine::begin() {
    
    engine->flags |= ENGINE_RUNNING;

    engine->prev_time.makeCurrent();
    engine->cur_time.makeCurrent();
      
    while(engine->flags & ENGINE_RUNNING) {
      loop();
    }
  }

  
}
