#include "Engine.hpp"
#include "Player.hpp"
#include "MonkeyHead.hpp"
#include "Input.hpp"
#include "Terrain.hpp"
#include "Sun.hpp"

using namespace peace;

i32 main() {

  f32 far_dist = 2000;
  
  Engine::init();
  Engine::engine->graphics.initCamera(degreesToRadians(60), 2.5, far_dist);
  
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
	  Engine::stop();
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
    
    Engine::emplaceDynamic<Player>(Vec3f(0,0,30));
    Engine::emplaceDynamic<Sun>(far_dist-200)->init();

    Terrain terrain;
    terrain.generate("Test2",Vec3f(0,0,0),Vec2u(2,2));
    terrain.loadFile("Test2");
				
    Engine::engine->graphics.back_color = Vec4f(0.4, 0.5, 1.0, 1.0);
    //Log::message(to_string(Engine::engine->static_container));
    Engine::begin();
    Log::message("Peace Be With You shut down sucessfully");

#ifndef N_DEBUG
  } catch(Exception e) {
    Log::error(e.what());
  }
#endif
  
  Engine::terminate();
  return EXIT_SUCCESS;
  
}
