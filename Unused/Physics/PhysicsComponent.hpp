#pragma once

#include "Component.hpp"
#include "Containers.hpp"
#include "Vector.hpp"

NAMESPACE {

  struct GameObject;

  PEACE_DEFINE_BITFLAGS(PhysicsComponentFlags, 8,
			PHYSICS_NO_FLAGS = 0x00,
			PHYSICS_STATIC = 0x01);

  struct MassData {

    f32 mass;
    f32 inv_mass;
    f32 inertia;
    f32 inv_inertia;
    
    MassData(f32 m, f32 i);
  };

  struct Material {
    
    f32 density;
    f32 cor; //coefficient of restitution
    
    Material(f32 mat_density, f32 mat_restitution);
    
    const static Material STATIC;
  };

  struct PhysicsComponent : Component {

    Material material;
    MassData mass_data;
    
    Vec3f veloc;
    Vec3f force;

    Quaternionf spin;

    PhysicsComponentFlags physics_flags;

    PhysicsComponent(GameObject* obj,
		     Material mat,
		     Vec3f xi,
		     Vec3f vi = Vec3f(0,0,0),
		     PhysicsComponentFlags _flags = PHYSICS_NO_FLAGS);
    void update(f32 dt);
    void applyForce(Vec3f force);
  };
  
}
