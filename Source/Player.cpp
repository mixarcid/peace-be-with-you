#include "Player.hpp"
#include "Assets.hpp"
#include "Input.hpp"
#include "Engine.hpp"
#include "PhysicsComp.hpp"

NAMESPACE {

  ChildTransform* Player::camera = NULL;
  Pointer<Player> Player::ptr;
  f32 Player::cam_speed(1);
  f32 Player::cam_rot_speed(0.001);
  Asset<BonedMeshBase> Player::mesh("SubjectB:Subject");
  
  void Player::init() {

    /*new (&tight_object.obb) BoundingOBB(*((BoundingOBB*)Player::mesh.get()->getTightBoundingObject()));
      new (&loose_object.sphere) BoundingSphere(*((BoundingSphere*)Player::mesh.get()->getLooseBoundingObject()));*/

    tight_object.set(Player::mesh.get()->getTightBoundingObject());
    loose_object.set(Player::mesh.get()->getLooseBoundingObject());
    

    addComponent(new BonedMesh(Player::mesh.get()));
    addComponent(new PhysicsComp(this,
				 Material(985, 0.5)));
    camera = addChildTransform(&engine->graphics.cam,
			       TransformBasic(Vec3f(0,-10,0)));
    
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
	/*Player::ptr->moveChildTransformAbs
	  (Player::camera, TransformBasic(Vec3f(0,0,0), q));*/
	Player::ptr->rotAbs(q);
	
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
    delete getComponent<PhysicsComp>();
  }

}
