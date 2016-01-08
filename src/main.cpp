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
  
  engine.begin();
  return EXIT_SUCCESS;
}
