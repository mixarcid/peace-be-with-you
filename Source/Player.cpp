#include "Player.hpp"
#include "Assets.hpp"
#include "Input.hpp"
#include "Engine.hpp"

NAMESPACE {

  Pointer<Player> Player::ptr;
  f32 Player::cam_speed(1);
  f32 Player::cam_rot_speed(0.001);
  Asset<BonedMeshBase> Player::mesh("SubjectB:Subject");
  
  void Player::init() {

    addComponent(new BonedMesh(Player::mesh.get()));
    addTransformChild(&engine->graphics.cam,
		      Transform(Vec3f(0,-3,2)));
    
    Input::addCursorPosCallback
      ("Main",
       [](GLFWwindow* win,
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
	Player::ptr->engine->graphics.cam.rot = q;
	
	prev_x = x;
	prev_y = y;
	  
      });

      
    Input::addKeyCallback
      ("Main",
       [](GLFWwindow* win,
	i32 key,
	i32 code,
	i32 act,
	i32 mods) {

	Mat3f coord = Player::ptr->engine->graphics.cam.getCoord();
	Vec3f right = coord.col(0);
	Vec3f dir = coord.col(1);
	
	switch(key) {
	case GLFW_KEY_W:
	  Player::ptr->transRel(dir*cam_speed);
	  break;
	case GLFW_KEY_S:
	  Player::ptr->transRel(-dir*cam_speed);
	  break;
	case GLFW_KEY_A:
	  Player::ptr->transRel(-right*cam_speed);
	  break;
	case GLFW_KEY_D:
	  Player::ptr->transRel(right*cam_speed);
	  break;
	case GLFW_KEY_SPACE:
	  if (act == GLFW_PRESS) {
	    BonedMesh* mesh = (Pointer<BonedMesh>)
	      Player::ptr->getComponent<RenderableComp>();
	    mesh->startAnimation("Walk");
	  }
	  break;
	}
      });
  }

  Player::~Player() {
    delete getComponent<RenderableComp>();
  }

}
