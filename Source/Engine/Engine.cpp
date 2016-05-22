#include "Engine.hpp"
#include "Input.hpp"
#include "Assets.hpp"
#include "BoundingObject.hpp"

NAMESPACE {

  Engine* Engine::engine;
  const static f32 ENGINE_QUADTREE_SIZE = 5000;
  const static u8 ENGINE_MAX_DEPTH = 8;

  Engine::Engine()
    : synchronized_callbacks(50),
    dynamic_container(20.0f),
    static_containers{
    QuadTree(BoundingAABB2D
	     (Vec2f(0,0),
	      Vec2f(ENGINE_QUADTREE_SIZE,ENGINE_QUADTREE_SIZE)),
	     ENGINE_MAX_DEPTH),
      QuadTree(BoundingAABB2D
	       (Vec2f(0,0),
		Vec2f(ENGINE_QUADTREE_SIZE,ENGINE_QUADTREE_SIZE)),
	       ENGINE_MAX_DEPTH)},
    graphics(this),
      physics(this),
      dt(0),
      cam_move_radius(300),
      flags(ENGINE_NO_FLAGS) {}

    Engine::~Engine() throw() {
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

      engine->window = gl::createWindow(Vec2s(1000,1000), "Peace be with You");
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

    void Engine::initCamPos(bool cur_container) {
      engine->getStaticContainer(cur_container)->clear();
      *(engine->getStaticContainer(cur_container)) =
	QuadTree(BoundingAABB2D
		 (engine->graphics.cam->getTrans().xy(),
		  Vec2f(ENGINE_QUADTREE_SIZE,ENGINE_QUADTREE_SIZE)),
		 ENGINE_MAX_DEPTH);
    }
    
    void Engine::_container_update() {
    
      while(true) {
	flag_mutex.lock();
	bool should_break =
	  !(engine->flags & ENGINE_CONTAINER_SWAP);
	flag_mutex.unlock();
	if (should_break) break;
      }

      initCamPos(false);
      for (auto& callback : scene_callbacks) {
	callback();
      }

      while(true) {
	engine->synchronized_mutex.lock();
	bool should_break = (engine->synchronized_callbacks.size() == 0);
	engine->synchronized_mutex.unlock();
	if (should_break) break;
      }

      for (StaticObject& obj : static_objects) {
	Pointer<StaticObject> p(&obj);
	registerStatic(p, false);
      }

      engine->synchronized_mutex.lock();
      engine->synchronized_callbacks.push_back([](){});
      engine->synchronized_mutex.unlock();
      while(true) {
	engine->synchronized_mutex.lock();
	bool should_break = (engine->synchronized_callbacks.size() == 0);
	engine->synchronized_mutex.unlock();
	if (should_break) break;
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

      Engine::engine->synchronized_mutex.lock();
      if (engine->synchronized_callbacks.size() > 0) {
	CallBack* fun = engine->synchronized_callbacks.begin();
	if (fun != engine->synchronized_callbacks.end()) {
	  CallBack f = *fun;
	  f();
	  engine->synchronized_callbacks.removeAndReplace(fun);
	}
      }
      Engine::engine->synchronized_mutex.unlock();
      
    
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

      engine->audio.update();

      //FPS calculation
      static Time last_second = engine->prev_time;
      static u32 num_frames = 0;
      static f32 fps = 0;

      ++num_frames;

      if (engine->cur_time.getMilliseconds() -
	  last_second.getMilliseconds() > 1000) {
	++engine->num_seconds;
	fps = num_frames;
	num_frames = 0;
	last_second = engine->cur_time;
	/*Log::message("FPS: %f", fps);
	Log::message("#collision checks: %u", num_checks);
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
		engine->_container_update();
		engine->prev_cam_pos = cam_pos;
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
