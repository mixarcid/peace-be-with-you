#pragma once

#include "Component.hpp"
#include "Containers.hpp"
#include "Vector.hpp"
#include "Quaternion.hpp"

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
    f32 cof_static; //static coefficient of friction
    f32 cof_dynamic; //dynamic coefficient of friction
    
    Material(f32 _density,
	     f32 _restitution,
	     f32 _cof_static,
	     f32 _cof_dynamic);
  };

  struct StaticMaterial : Material {
    StaticMaterial(f32 cof_static,
		   f32 cof_dynamic);
  };

  struct PhysicsComp : Component {

    Material material;
    MassData mass_data;
    
    Vec3f veloc;
    Vec3f force;

    Quaternionf spin;

    PhysicsComp(GameObject* object,
		Material mat,
		Vec3f vi = Vec3f(0,0,0))
      : material(mat),
	mass_data(0,0),
	veloc(vi),
	force(Vec3f(0,0,0)) {
      $rttiConstruct("PhysicsComp");
      init(object);
    }
    void init(GameObject* object);
    void update(f32 dt);
    void applyImpulse(Vec3f j);
  };
  $registerRttiStruct();
  
}
