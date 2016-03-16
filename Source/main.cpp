#include "Engine.hpp"
#include "Player.hpp"
#include "MonkeyHead.hpp"
#include "Input.hpp"
#include "Terrain.hpp"

using namespace peace;

i32 main() {
  
  Engine::init();
      
  //try {

  Engine::engine->graphics.ambient = 0;
  Engine::engine->graphics.emplaceDirLight(Vec3f(-1,-1,1),
				    Vec3f(1,1,1));
    
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
    terrain.generate(Vec3f(0,0,0),Vec2u(5,5));
    
    Engine::emplaceDynamic<Player>(Vec3f(0,0,30));
    Player::ptr->getComponent<DynamicPhysicsComp>()
      ->veloc = Vec3f(0,10,0);
    Engine::emplaceDynamic<MonkeyHead>(Vec3f(0,1,30),
				       Vec3f(0,10,0));
    Engine::begin();
    //printf("%d vs %d\n", (i32) 1 & 255, (i32) -1.01 & 255);
    
    /*} catch(Exception e) {
    Log::error(e.what());
    }*/

    Log::message("Peace Be With You shut down sucessfully");
    Engine::terminate();
  return EXIT_SUCCESS;
  
}
