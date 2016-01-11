#include "Engine.hpp"
#include "GameObject.hpp"

NAMESPACE {

  Engine::Engine()
    : system_manager({}),
    dt(0) {
    
    Log::init(NULL);
    system_manager.addSystem(&Log::logger);
    system_manager.start();
    
    gl::init();

    window = gl::createWindow(Vec2s(700,700), "Peace be with You");
    Input::init(window);
    Input::setManager("Main");
    Input::addFlags("Main", INPUT_CURSOR_DISABLED);
  
    const unsigned char* version = glGetString(GL_VERSION);
    fatalAssert(version != NULL,
		"Cannot determine OpenGL version");
    Log::message("Your OpenGL version is %s", version);
    
    graphics = new Graphics("Toon",Vec4f(1,1,1,1));
    
    flags |= ENGINE_GRAPHICS_INIT;
    
    Asset::loadAll();
    flags |= ENGINE_ASSETS_LOADED;

    physics = new Physics(Vec3f(0,0,0),
			  Vec3f(5000,5000,5000));
  }
  
  GameObjectHandle Engine::addGameObject(GameObject* object) {
    GameObjectHandle h;
    h.flags = object->object_flags;
    if (h.flags & OBJECT_NAMED) {
      h.object_name = ((NamedObject*)object)->name;
      named_objects[h.object_name] = object;
    }
    if (h.flags & OBJECT_UPDATE) {
      h.update_handle = update_objects.push_back(object);
    }
    h.physics_handle = physics->addObject(object);
    h.graphics_handle = graphics->addNode(object);
    return h;
  }
  
  void Engine::removeGameObject(GameObjectHandle h) {
    if (h.flags & OBJECT_NAMED) {
      named_objects.erase(h.object_name);
    }
    if (h.flags & OBJECT_UPDATE) {
      update_objects.removeAndReplace(h.update_handle);
    }
    physics->removeObject(h.physics_handle);
    graphics->removeNode(h.graphics_handle);
  }
  
  void Engine::loop() {
    
    if (glfwWindowShouldClose(window)) {
      flags &= ~ENGINE_RUNNING;
      return;
    }

    cur_time.makeCurrent();
    dt = (f32) (cur_time.getMilliseconds()
		- prev_time.getMilliseconds())/1000;

    physics->update(dt);

    for (GameObject* obj : update_objects) {
      obj->update();
    }
    
    graphics->render(window,dt);
    gl::checkError();
      
    prev_time = cur_time;
  }

  void Engine::begin() {
    
    flags |= ENGINE_RUNNING;

    prev_time.makeCurrent();
    cur_time.makeCurrent();
      
    while(flags & ENGINE_RUNNING) {
      loop();
    }

    glfwDestroyWindow(window);
    
  }
  
  Engine::~Engine() {
    if (flags & ENGINE_ASSETS_LOADED) {
      Asset::freeAll();
    }
    if (flags & ENGINE_GRAPHICS_INIT) {
      delete graphics;
      gl::terminate();
    }
    delete physics;
    Log::terminate();
  }
  
}
