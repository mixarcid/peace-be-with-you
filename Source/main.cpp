#include "Engine.hpp"
#include "Player.hpp"
#include "Input.hpp"

using namespace peace;

i32 main() {
  Engine engine;
  try {
    engine.init();
    engine.graphics.background_color = Vec4f(0,0,0,1);
    engine.graphics.ambient = 0;
    engine.graphics.emplaceDirLight(Vec3f(0,0,1),
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
	}
      });
    
    engine.emplaceObject<Player>(Vec3f(0,10,0));
    engine.begin();
    return EXIT_SUCCESS;
  } catch(Exception e) {
    Log::error(e.what());
  }
}
