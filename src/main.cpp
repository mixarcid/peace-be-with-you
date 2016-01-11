#include "Engine.hpp"
#include "Player.hpp"
#include "Terrain.hpp"

using namespace peace;

i32 main() {
  Engine engine;
  
  DirLight light(Vec3f(0,0,1),
		 Vec3f(1,1,1));
  engine.graphics->addDirLight(&light);
  engine.graphics->setAmbient(0);
  
  Player p(Vec3f(0,0,0), &engine);
  
  Terrain t(&engine);
  t.generate(Vec2<u32>(1,1));

  Input::addKeyCallback
    ("Main",
     []
     (GLFWwindow* win,
      i32 key,
      i32 code,
      i32 act,
      i32 mods) {
      switch(key) {
      case GLFW_KEY_1:
	gl::setDrawMode(GL_TRIANGLES);
	break;
      case GLFW_KEY_2:
	gl::setDrawMode(GL_POINTS);
	break;
      case GLFW_KEY_3:
	gl::setDrawMode(GL_LINES);
	break;
      }
    });
  Input::setManager("Main");
  
  engine.begin();
  return EXIT_SUCCESS;
}
