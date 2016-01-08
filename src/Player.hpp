#pragma once

#include "GameObject.hpp"
#include "Engine.hpp"
#include "Camera.hpp"
#include "MonkeyHead.hpp"


NAMESPACE {

  struct Player : NamedObject {

    Camera cam;
    f32 cam_speed;
    f32 cam_rot_speed;
    Array<MonkeyHead*> heads; 
    
    Player(Vec3f pos,
	   Engine* engine)
      : NamedObject("Player",
		    &mesh,
		    Material(1, 0.5),
		    pos,
		    Vec3f(0,0,0),
		    PHYSICS_NO_FLAGS,
		    OBJECT_UPDATE),
	cam(degreesToRadians(60), 1, 50),
	cam_speed(5),
	cam_rot_speed(0.001) {

      $rttiConstruct("Player");

      engine->graphics->setCamera(&cam);   

      Input::addCursorPosCallback
	([this]
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
	([this]
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
	    
      GameObject::init(engine);
    }

    virtual void update();
    virtual ~Player();
    
    static BonedMesh mesh;

  };
  $registerRttiStruct();
}

