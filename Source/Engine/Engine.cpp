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
    engine->dynamic_container.update(obj);
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
    //Log::message("Dynamic objects: %u", engine->dynamic_objects.size());
    
    u32 num_checks = 0;
    u32 num_collisions = 0;

    engine->physics.updateVsStatic(&num_checks, &num_collisions);
    engine->graphics.initRender();
    engine->graphics.renderDynamic();

    Thread dyn([&num_checks, &num_collisions]() {
	engine->physics.updateVsDynamic(&num_checks, &num_collisions);
      });
    
    engine->graphics.renderStatic();
    engine->graphics.finalizeRender();
    gl::checkError();
    
    dyn.join();

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
      Log::message("FPS: %f", fps);
      Log::message("#collision checks: %u", num_checks);
      Log::message("#collisions: %u", num_collisions);
      Log::message("#dynamic objects: %u", engine->dynamic_objects.size());
    }
    engine->prev_time = engine->cur_time;
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
