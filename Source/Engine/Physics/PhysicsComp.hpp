#pragma once

#include "Component.hpp"
#include "Containers.hpp"
#include "Vector.hpp"
#include "Quaternion.hpp"
#include "GameObject.hpp"

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

  struct StaticPhysicsComp : Component {
    
    Material material;
    MassData mass_data;

    StaticPhysicsComp(Pointer<StaticObject> object,
		      Material mat)
      : material(mat),
	mass_data(0,0) {
      $rttiConstruct("StaticPhysicsComp");
      init((Pointer<GameObject>)object);
    }

    StaticPhysicsComp(Pointer<GameObject> object,
		      Material mat)
      : material(mat),
	mass_data(0,0) {
      $rttiConstruct("StaticPhysicsComp");
      init(object);
    }
    
    void init(Pointer<GameObject> object);
    Vec3f getVeloc();
    void applyImpulse(Vec3f j) {}
    void onMove(Pointer<StaticObject> object) {}
    void onStop(Pointer<StaticObject> object) {}
    bool isMoving();
    
  };
  $registerRttiStruct();

  struct DynamicPhysicsComp : StaticPhysicsComp {

    typedef Array<DynamicComponentPairP<DynamicPhysicsComp>> MovingObjectArray;
    typedef Pointer<DynamicComponentPairP<DynamicPhysicsComp>> MovingObjectHandle;
    
    Vec3f veloc;
    Vec3f prev_veloc;
    Vec3f force;

    Quaternionf spin;

    MovingObjectHandle moving_object_handle;

    DynamicPhysicsComp(Pointer<DynamicObject> object,
		       Material mat,
		       Vec3f vi = Vec3f(0,0,0))
      : StaticPhysicsComp((Pointer<GameObject>)object, mat),
	veloc(vi),
	prev_veloc(FLT_MAX, FLT_MAX, FLT_MAX),
	force(Vec3f(0,0,0)) {
      onMove(object);
      $rttiConstruct("DynamicPhysicsComp");
    }

    Vec3f getVeloc();
    
    void update(Pointer<DynamicObject> object, f32 dt);
    void applyImpulse(Vec3f j);
    void onMove(Pointer<DynamicObject> object);
    void onStop(Pointer<DynamicObject> object);
    bool isMoving();

    void _on_move();
    
  };
  $registerRttiStruct();

}
