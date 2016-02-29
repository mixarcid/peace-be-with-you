#include "Engine.hpp"
#include "Input.hpp"
#include "Assets.hpp"
#include "BoundingObject.hpp"

NAMESPACE {

  Engine::Engine()
    : graphics(this),
    physics(this),
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
  
  void Engine::loop() {

    if (glfwWindowShouldClose(window)) {
      flags &= ~ENGINE_RUNNING;
      return;
    }
    
    cur_time.makeCurrent();
    dt = (f32) (cur_time.getMilliseconds()
		- prev_time.getMilliseconds())/1000;
    physics.update();
    graphics.render();
    gl::checkError();

    glfwPollEvents();

    //FPS calculation
    static Time last_second = prev_time;
    static u32 num_frames = 0;
    static f32 fps = 0;

    ++num_frames;

    if (cur_time.getMilliseconds() -
	last_second.getMilliseconds() > 1000) {
      fps = num_frames;
      num_frames = 0;
      last_second = cur_time;
    }
    Log::message("FPS: %f", fps);
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
