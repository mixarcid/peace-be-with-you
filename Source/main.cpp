#include "Engine.hpp"
#include "Player.hpp"
#include "MonkeyHead.hpp"
#include "Input.hpp"
#include "Terrain.hpp"
#include "Sun.hpp"

using namespace peace;

i32 main() {
  
  Engine::init();

#ifndef N_DEBUG
  try {
#endif

    Engine::engine->graphics.ambient = 0;
    
    Input::setManager("Main");
    Input::addFlags("Main", INPUT_CURSOR_DISABLED);
    Input::addKeyCallback
      ("Main",
       [](GLFWwindow* win, i32 key,
	  i32 code, i32 act, i32 mods) {
	switch(key) {
	case GLFW_KEY_ESCAPE:
	  Engine::engine->flags &= ~ENGINE_RUNNING;
	  break;
	case GLFW_KEY_1:
	  gl::setGraphicsMode(PEACE_GL_TRIANGLES);
	  break;
	case GLFW_KEY_2:
	  gl::setGraphicsMode(PEACE_GL_LINES);
	  break;
	case GLFW_KEY_3:
	  gl::setGraphicsMode(PEACE_GL_POINTS);
	  break;
	case GLFW_KEY_4:
	  if (act == GLFW_PRESS) {
	    Engine::engine->graphics.flags ^=
	      GRAPHICS_RENDER_BOUNDING_TIGHT;
	    Engine::engine->graphics.flags ^=
	      GRAPHICS_RENDER_BOUNDING_LOOSE;
	  }
	  break;
	}
      });

    Terrain terrain;
    //terrain.generate("Test",Vec3f(0,0,0),Vec2u(7,7));
    terrain.loadFile("Test");
    for (u32 chunk_x = 0; chunk_x < terrain.size.x(); ++chunk_x) {
      for (u32 chunk_y = 0; chunk_y < terrain.size.y(); ++chunk_y) {
	terrain.loadChunk(Vec2u(chunk_x,chunk_y));
      }
    }
    
    Engine::emplaceDynamic<Player>(Vec3f(0,0,30));
    Engine::emplaceDynamic<Sun>()->init((Pointer<DynamicObject>&)Player::ptr);
				
    Engine::engine->graphics.back_color = Vec4f(0.3, 0.3, 1.0, 1.0);
    //Log::message(to_string(Engine::engine->static_container));
    Engine::begin();

#ifndef N_DEBUG
  } catch(Exception e) {
    Log::error(e.what());
  }
#endif
  
  Log::message("Peace Be With You shut down sucessfully");
  Engine::terminate();
  return EXIT_SUCCESS;
  
}
