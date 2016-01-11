#include "Player.hpp"
#include "MeshLoader.hpp"

NAMESPACE {

  BonedMesh Player::mesh;
  static MeshLoader* loader;

  CONSTRUCT_ASSET(PLAYER_LOADER);
 
  loader = new MeshLoader("SubjectB");
  Player::mesh = loader->getBonedMesh("Subject");
  
  DELETE_ASSET;
 
  delete loader;
  
  END_ASSET;

  void Player::init() {
    
      this->engine->graphics->setCamera(&cam);   

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
	  cam.rotateAbs(q);
	
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

	  Mat3f coord = cam.getCoord();
	  Vec3f right = coord.col(0);
	  Vec3f dir = coord.col(1);
	
	  switch(key) {
	  case GLFW_KEY_W:
	    translateRel(dir*cam_speed);
	    break;
	  case GLFW_KEY_S:
	    translateRel(-dir*cam_speed);
	    break;
	  case GLFW_KEY_A:
	    translateRel(-right*cam_speed);
	    break;
	  case GLFW_KEY_D:
	    translateRel(right*cam_speed);
	    break;
	  case GLFW_KEY_ESCAPE:
	    this->engine->flags &= ~ENGINE_RUNNING;
	    break;
	  case GLFW_KEY_SPACE:
	    if (act == GLFW_PRESS) {
	      heads.push_back(new MonkeyHead(cam.trans,
					     dir*10,
					     this->engine));
	    }
	    break;
	  }
	});
  }

  void Player::update() {
    cam.translateAbs(this->trans+Vec3f(0,3,3));
  }

  Player::~Player() {
    for (MonkeyHead* head : heads) {
      delete head;
    }
  }

}
