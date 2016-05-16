#include "Player.hpp"
#include "Assets.hpp"
#include "Input.hpp"
#include "Engine.hpp"
#include "PhysicsComp.hpp"
#include "MonkeyHead.hpp"

NAMESPACE {

  ChildObject* Player::camera = NULL;
  Pointer<Player> Player::ptr;
  //Vec3f(0,0,5) for first person
  Transform Player::camera_diff(Vec3f(0,10,7),
				Quaternionf
				(0,0,degreesToRadians(180)));
  f32 Player::cam_speed(100);
  f32 Player::cam_rot_speed(0.001);
  Asset<BonedMeshBase> Player::mesh("Granny:Granny");
  
  void Player::init() {

    tight_object.set(mesh.get()->getTightBoundingObject());
    loose_object.set(mesh.get()->getLooseBoundingObject());
    getTightBoundingObject()->transform(getTransform());
    getLooseBoundingObject()->transform(getTransform());

    addComponent(new BonedMesh(Player::mesh.get()));
    Pointer<DynamicObject> obj(this);
    addComponent(new DynamicPhysicsComp(obj,Material(985, 0.1, 0.5, 0.4)));
    camera = addChild((Pointer<DynamicObject>&)Engine::engine->graphics.cam,
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
	
	Quaternionf qy(y_tot*cam_rot_speed,0,0);
	Quaternionf qx(0,0,-x_tot*cam_rot_speed);

	Vec3f z_correct(0,0,camera_diff.getTrans().z());

	Player::ptr->moveChildAbs
	  (Player::camera, Transform::combine
	   (Transform(z_correct),
	    Transform(Vec3f(0,0,0), qy),
	    camera_diff,
	    Transform(-z_correct)));
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
	  auto p = Engine::emplaceDynamic
	    <MonkeyHead>(Player::ptr->getTrans()+dir*4 + Vec3f(0,0,3), dir*20);
	  p->rotAbs(Player::ptr->getRot());
	  BonedMesh* mesh = (Pointer<BonedMesh>)
	    Player::ptr->getComponent<RenderableComp>();
	  mesh->startAnimation("Throw");
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

	Pointer<DynamicPhysicsComp> phys = Player::ptr->getComponent<DynamicPhysicsComp>();
	f32 z = phys->veloc.z();
	
	if (act != GLFW_RELEASE) {
	  
	  BonedMesh* mesh = (Pointer<BonedMesh>)
	    Player::ptr->getComponent<RenderableComp>();
	  if (!(mesh->cur_animation.flags & ANIMATION_PLAYING)) {
	    switch(key) {
	    case GLFW_KEY_W:
	    case GLFW_KEY_A:
	    case GLFW_KEY_S:
	    case GLFW_KEY_D:
	      mesh->loopAnimation("Run");
	      break;
	    }
	  }
	  
	  switch(key) {
	  case GLFW_KEY_W:
	    phys->veloc = (dir*cam_speed);
	    Player::ptr->onMove();
	    phys->veloc.z() = z;
	    break;
	  case GLFW_KEY_S:
	    phys->veloc = (-dir*cam_speed);
	    Player::ptr->onMove();
	    phys->veloc.z() = z;
	    break;
	  case GLFW_KEY_A:
	    phys->veloc = (-right*cam_speed);
	    Player::ptr->onMove();
	    phys->veloc.z() = z;
	    break;
	  case GLFW_KEY_D:
	    phys->veloc = (right*cam_speed);
	    Player::ptr->onMove();
	    phys->veloc.z() = z;
	    break;
	  case GLFW_KEY_SPACE:
	    Player::ptr->transRel(Vec3f(0,0,0.01));
	    phys->veloc+=Vec3f(0,0,20);
	    break;
	  }

	} else {

	  switch(key) {
	  case GLFW_KEY_W:
	  case GLFW_KEY_S:
	  case GLFW_KEY_A:
	  case GLFW_KEY_D:
	    BonedMesh* mesh = (Pointer<BonedMesh>)
	      Player::ptr->getComponent<RenderableComp>();
	    phys->veloc.x() = 0;
	    phys->veloc.y() = 0;
	    mesh->stopAnimation();
	    break;
	  }
	  
	}
      });
  }

  Player::~Player() {
    delete getComponent<DynamicPhysicsComp>();
    delete getComponent<RenderableComp>();
  }

}
