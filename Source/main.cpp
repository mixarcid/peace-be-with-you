#include "Engine.hpp"
#include "Player.hpp"
#include "Input.hpp"
#include "Terrain.hpp"

using namespace peace;

i32 main() {
  
  Engine engine;
  engine.init();
      
  try {

    engine.graphics.ambient = 0;
    engine.graphics.emplaceDirLight(Vec3f(-1,-1,1),
				    Vec3f(1,1,1));
    
    Input::setManager("Main");
    Input::addFlags("Main", INPUT_CURSOR_DISABLED);
    Input::addKeyCallback
      ("Main",
       [&engine]
       (GLFWwindow* win, i32 key,
	i32 code, i32 act, i32 mods) {
	switch(key) {
	case GLFW_KEY_ESCAPE:
	  engine.flags &= ~ENGINE_RUNNING;
	  break;
	case GLFW_KEY_1:
	  gl::setDrawMode(GL_TRIANGLES);
	  break;
	case GLFW_KEY_2:
	  gl::setDrawMode(GL_LINES);
	  break;
	case GLFW_KEY_3:
	  gl::setDrawMode(GL_POINTS);
	  break;
	}
      });

    Terrain terrain(&engine);
    terrain.generate(Vec2u(5,5));
    
    engine.emplaceObject<Player>(Vec3f(0,10,0));
    engine.begin();
    
  } catch(Exception e) {
    Log::error(e.what());
  }

  Log::message("Peace Be With You shut down sucessfully");
  return EXIT_SUCCESS;
  
}
