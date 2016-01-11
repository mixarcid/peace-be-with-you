#pragma once

#include "PhysicalObject.hpp"
#include "GameObjectContainer.hpp"

NAMESPACE {

  /*remember that all GameObjects must also 
    be proper RTTI objects
  */

  struct Engine;
  
  PEACE_DEFINE_BITFLAGS(GameObjectFlags, 8,
			OBJECT_NO_FLAGS = 0x00,
		        OBJECT_UPDATE = 0x01,
			OBJECT_NAMED = 0x02,
			OBJECT_SAVEABLE = 0x04,
			OBJECT_PHYSICS = 0x08,
			OBJECT_GRAPHICS = 0x10);
  
  struct GameObject : PhysicalObject {

    GameObjectFlags object_flags;
    Engine* engine;
    GameObjectHandle handle;
    
    GameObject(Renderable* rend,
	       Material mat,
	       Vec3f xi,
	       Vec3f vi = Vec3f(0,0,0),
	       PhysicalObjectFlags physics_flags = PHYSICS_NO_FLAGS,
	       GameObjectFlags flags = OBJECT_NO_FLAGS);
	       
    void init(Engine* engine);
    virtual void update() {}
    virtual ~GameObject();
  };

  struct NamedObject : GameObject {

    const String name;
    NamedObject(String _name,
		Renderable* rend,
		Material mat,
		Vec3f xi,
		Vec3f vi = Vec3f(0,0,0),
		PhysicalObjectFlags physics_flags = PHYSICS_NO_FLAGS,
		GameObjectFlags flags = OBJECT_NO_FLAGS);
  };
  
}
