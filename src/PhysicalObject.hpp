#pragma once

#include "Node.hpp"
#include "Containers.hpp"

NAMESPACE {

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

  struct PhysicalObject : Node {

    Material material;
    MassData mass_data;
    
    Vec3f veloc;
    Vec3f force;

    Quaternionf spin;

    PhysicalObject(Renderable* rend, Material mat,
		  Vec3f xi, Vec3f vi = Vec3f(0,0,0));
    void update(f32 dt);
    void applyForce(Vec3f force);
    
  };

  struct StaticObject : PhysicalObject {
    StaticObject(Renderable* rend, Vec3f xi, Vec3f vi = Vec3f(0,0,0));
  };
  
}
