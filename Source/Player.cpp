#include "Player.hpp"
#include "Assets.hpp"
#include "Input.hpp"
#include "Engine.hpp"
#include "PhysicsComp.hpp"
#include "MonkeyHead.hpp"

NAMESPACE {

  ChildTransform* Player::camera = NULL;
  Pointer<Player> Player::ptr;
  TransformBasic Player::camera_diff(Vec3f(0,10,0),
				     Quaternionf
				     (0,0,degreesToRadians(180)));
  f32 Player::cam_speed(1);
  f32 Player::cam_rot_speed(0.001);
  Asset<BonedMeshBase> Player::mesh("SubjectB:Subject");
  
  void Player::init() {

    tight_object.set(mesh.get()->getTightBoundingObject());
    loose_object.set(mesh.get()->getLooseBoundingObject());
    

    addComponent(new BonedMesh(Player::mesh.get()));
    addComponent(new PhysicsComp(this,
				 Material(985, 0.1, 0.5, 0.4)));
    
    camera = addChildTransform(&engine->graphics.cam,
			       camera_diff);
    
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
	
	Quaternionf qy(-y_tot*cam_rot_speed,0,0);
	Quaternionf qx(0,0,-x_tot*cam_rot_speed);

	Player::ptr->moveChildTransformAbs
	  (Player::camera, TransformBasic::combine
	   (TransformBasic(Vec3f(0,0,0), qy), camera_diff));
	Player::ptr->rotAbs(qx);
	  
	prev_x = x;
	prev_y = y;
	  
      });

    Input::addMouseButtonCallback
      ("Main",
       [](GLFWwindow* win,
	  i32 button, i32 act, i32 mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT &&
	    act == GLFW_PRESS) {
	  Vec3f dir = -(Player::ptr->getRot()*Vec3f(0,1,0));
	  dir.normalize();
	  Player::ptr->engine->emplaceDynamic
	    <MonkeyHead>(Player::ptr->getTrans()+dir*5, dir*20);
	}
      });

      
    Input::addKeyCallback
      ("Main",
       [](GLFWwindow* win,
	i32 key,
	i32 code,
	i32 act,
	i32 mods) {

	Vec3f dir = -(Player::ptr->getRot()*Vec3f(0,1,0));
	dir.normalize();
	Vec3f right = Vec3f::cross(dir, Vec3f(0,0,1));

	Pointer<PhysicsComp> phys = Player::ptr->getComponent<PhysicsComp>();
	
	switch(key) {
	case GLFW_KEY_W:
	  Player::ptr->transRel(dir*cam_speed);
	  phys->veloc.x() = phys->veloc.y() = 0;
	  break;
	case GLFW_KEY_S:
	  Player::ptr->transRel(-dir*cam_speed);
	  phys->veloc.x() = phys->veloc.y() = 0;
	  break;
	case GLFW_KEY_A:
	  Player::ptr->transRel(-right*cam_speed);
	  phys->veloc.x() = phys->veloc.y() = 0;
	  break;
	case GLFW_KEY_D:
	  Player::ptr->transRel(right*cam_speed);
	  phys->veloc.x() = phys->veloc.y() = 0;
	  break;
	case GLFW_KEY_SPACE:
	  if (act == GLFW_PRESS) {
	    /*BonedMesh* mesh = (Pointer<BonedMesh>)
	      Player::ptr->getComponent<RenderableComp>();
	      mesh->startAnimation("Walk");*/
	    phys->veloc+=Vec3f(0,0,20);
	  }
	  break;
	}
      });
  }

  Player::~Player() {
    delete getComponent<RenderableComp>();
    delete getComponent<PhysicsComp>();
  }

}
