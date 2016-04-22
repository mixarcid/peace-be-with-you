#include "Engine.hpp"
#include "Input.hpp"
#include "Assets.hpp"
#include "BoundingObject.hpp"

NAMESPACE {

  Engine* Engine::engine;

  Engine::Engine()
    : dynamic_container(20.0f),
    static_containers{
    QuadTree(BoundingAABB2D
	     (Vec2f(0,0),
	      Vec2f(10000,10000)),
	     8),
      QuadTree(BoundingAABB2D
	       (Vec2f(0,0),
		Vec2f(10000,10000)),
	       8)},
    graphics(this),
    physics(this),
    dt(0),
    cam_move_radius(100),
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

  QuadTree* Engine::getStaticContainer(bool cur_container) {
    QuadTree* ret;
    flag_mutex.lock();
    if (engine->flags & ENGINE_CONTAINER_FIRST) {
      ret = &static_containers[cur_container];
    } else {
      ret = &static_containers[!cur_container];
    }
    flag_mutex.unlock();
    return ret;
  }
  
  void Engine::swapContainers() {
    flag_mutex.lock();
    engine->flags ^= ENGINE_CONTAINER_FIRST;
    engine->flags |= ENGINE_CONTAINER_SWAP;
    flag_mutex.unlock();
  }

  
  void Engine::registerMove(Pointer<DynamicObject>& obj) {
    engine->dynamic_container.update(obj);
  }

  void Engine::_container_update() {
    while(true) {
      flag_mutex.lock();
      bool should_break =
	!(engine->flags & ENGINE_CONTAINER_SWAP);
      flag_mutex.unlock();
      if (should_break) break;
    }
    getStaticContainer(false)->clear();
    for (StaticObject& obj : static_objects) {
      Pointer<StaticObject> p(&obj);
      registerStatic(p, false);
    }
    swapContainers();
  }
  
  void Engine::loop() {

    if (glfwWindowShouldClose(engine->window)) {
      engine->flag_mutex.lock();
      engine->flags &= ~ENGINE_RUNNING;
      engine->flag_mutex.unlock();
      return;
    }
    
    engine->cur_time.makeCurrent();
    engine->dt = (f32)
      (engine->cur_time.getMilliseconds()
       - engine->prev_time.getMilliseconds())/1000;
    //Log::message("Dynamic objects: %u", engine->dynamic_objects.size());
    
    u32 num_checks = 0;
    u32 num_collisions = 0;

    //engine->physics.updateVsStatic(&num_checks, &num_collisions);
    // Log::message("Static collision checks: %u", num_checks);
    engine->graphics.initRender();
    engine->graphics.renderDynamic();

    Thread dyn([&num_checks, &num_collisions]() {
	engine->physics.updateVsDynamic(&num_checks, &num_collisions);
      });
    
    engine->graphics.renderStatic();
    dyn.join();

    Thread stat([&num_checks, &num_collisions]() {
	engine->physics.updateVsStatic(&num_checks, &num_collisions);
      });
    
    engine->graphics.finalizeRender();
    stat.join();
    
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
      Log::message("FPS: %f", fps);
	/*Log::message("#collision checks: %u", num_checks);
      Log::message("#collisions: %u", num_collisions);
      Log::message("#dynamic objects: %u", engine->dynamic_objects.size());*/
    }
    engine->prev_time = engine->cur_time;
  }

  void Engine::begin() {
    
    engine->flags |= ENGINE_RUNNING;

    engine->prev_time.makeCurrent();
    engine->cur_time.makeCurrent();

    if (engine->graphics.cam) {
      engine->prev_cam_pos = engine->graphics.cam->getTrans().xy();
    }

    Thread container_thread
      ([]() {
	while(true) {

	  engine->flag_mutex.lock();
	  bool should_break =
	    !(engine->flags & ENGINE_RUNNING);
	  engine->flag_mutex.unlock();
	  if (should_break) break;
	  
	  if (engine->graphics.cam) {
	    Vec2f cam_pos = engine->graphics.cam->getTrans().xy();
	    if ((cam_pos - engine->prev_cam_pos).norm() >
		engine->cam_move_radius) {
	      //Log::message("Begun container update");
	      engine->_container_update();	      
	      engine->prev_cam_pos = cam_pos;
	      //Log::message("Finished container update");
	    }
	  }
	}
      });
      
    while(true) {
      engine->flag_mutex.lock();
      bool should_break =
	!(engine->flags & ENGINE_RUNNING);
      engine->flag_mutex.unlock();
      if (should_break) break;
      loop();
      engine->flag_mutex.lock();
      engine->flags &= ~ENGINE_CONTAINER_SWAP;
      engine->flag_mutex.unlock();
    }
    container_thread.join();
  }

  void Engine::stop() {
    engine->flag_mutex.lock();
    engine->flags &= ~ENGINE_RUNNING;
    engine->flag_mutex.unlock();
  }

  
}
