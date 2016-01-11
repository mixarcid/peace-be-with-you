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
	cam_speed(1),
	cam_rot_speed(0.001) {

      $rttiConstruct("Player");
      GameObject::init(engine);
      init();
    }

    void init();
    virtual void update();
    virtual ~Player();
    
    static BonedMesh mesh;

  };
  $registerRttiStruct();
}

