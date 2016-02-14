#include "Engine.hpp"
#include "Input.hpp"
#include "Assets.hpp"

NAMESPACE {

  Engine::Engine()
    : graphics(this),
    dt(0),
    flags(ENGINE_NO_FLAGS) {}

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
    Log::init(NULL);
    system_manager.addSystem(Log::getSystem());
    system_manager.start();

    gl::init();

    window = gl::createWindow(Vec2s(700,700), "Peace be with You");
    Input::init(window);
    
    const unsigned char* version = glGetString(GL_VERSION);
    fatalAssert(version != NULL,
		"Cannot determine OpenGL version");
    Log::message("Your OpenGL version is %s", version);
    
    graphics.init(window);

    flags |= ENGINE_GRAPHICS_INIT;

    loadAllAssets();
    flags |= ENGINE_ASSETS_LOADED;
    
  }

  void Engine::removeObject(Pointer<GameObject> handle) {
    game_objects.removeAndReplace(handle);
  }
  
  void Engine::loop() {

    if (glfwWindowShouldClose(window)) {
      flags &= ~ENGINE_RUNNING;
      return;
    }
    
    cur_time.makeCurrent();
    dt = (f32) (cur_time.getMilliseconds()
		- prev_time.getMilliseconds())/1000;

    graphics.render();
    gl::checkError();

    glfwPollEvents();
    
    prev_time = cur_time;
  }

  void Engine::begin() {
    
    flags |= ENGINE_RUNNING;

    prev_time.makeCurrent();
    cur_time.makeCurrent();
      
    while(flags & ENGINE_RUNNING) {
      loop();
    }
  }

  
}
