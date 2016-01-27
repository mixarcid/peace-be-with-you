#include "Player.hpp"
#include "Assets.hpp"
#include "Input.hpp"
#include "Engine.hpp"

NAMESPACE {
  
  f32 Player::cam_speed(1);
  f32 Player::cam_rot_speed(0.001);
  StaticMesh* Player::mesh;
  static MeshLoader* loader;

  CONSTRUCT_ASSET(PLAYER_LOADER);
 
  loader = new MeshLoader("Monkey");
  Player::mesh = loader->getStaticMesh("Suzanne");
  
  DELETE_ASSET;
 
  delete loader;
  
  END_ASSET;

  void Player::init() {

    addComponent(Player::mesh);
    
    Input::addCursorPosCallback
      ("Main",
       [this]
       (GLFWwindow* win,
	f64 x, f64 y) {
	
	static f64 prev_x = x;
	static f64 prev_y = y;
	static f32 x_tot = 0;
	static f32 y_tot = 0;

	f64 dx = x - prev_x;
	f64 dy = y - prev_y;
	x_tot += dx;
	y_tot += dy;
	
	Quaternionf q(-y_tot*cam_rot_speed,
		      0,
		      -x_tot*cam_rot_speed);
	engine->graphics.cam.rot = q;
	
	prev_x = x;
	prev_y = y;
	  
      });

      
    Input::addKeyCallback
      ("Main",
       [this]
       (GLFWwindow* win,
	i32 key,
	i32 code,
	i32 act,
	i32 mods) {

	Mat3f coord = engine->graphics.cam.getCoord();
	Vec3f right = coord.col(0);
	Vec3f dir = coord.col(1);
	
	switch(key) {
	case GLFW_KEY_W:
	  setTrans(getTrans()+dir*cam_speed);
	  break;
	case GLFW_KEY_S:
	  setTrans(getTrans()-dir*cam_speed);
	  break;
	case GLFW_KEY_A:
	  setTrans(getTrans()-right*cam_speed);
	  break;
	case GLFW_KEY_D:
	  setTrans(getTrans()+right*cam_speed);
	  break;
	case GLFW_KEY_SPACE:
	  break;
	}
      });
  }

}
