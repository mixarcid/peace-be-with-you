#include "Physics.hpp"
#include "Collision.hpp"
#include "Engine.hpp"

NAMESPACE {

  Physics::Physics(Engine* _engine)
    : engine(_engine), gravity(Vec3f(0,0,-9.8)) {}

  void Physics::updateVsDynamic(u32* num_checks, u32* num_collisions) {

    static u32 counter = 0;
    counter = counter > 7 ? 0 : counter + 1;

    const f32 dt = engine->dt;

    for (auto& obj : moving_objects) {

      auto& comp = obj->getComponent<DynamicPhysicsComp>();
      
      engine->traverseNeighbors<DynamicPhysicsComp>
	(obj,
	 [&obj, &comp, num_checks, num_collisions]
	 (Pointer<DynamicObject>& obj2,
	  Pointer<DynamicPhysicsComp>& comp2) -> bool {
	  ++(*num_checks);
	  if (resolveCollision(obj, comp, obj2, comp2)) {
	    ++(*num_collisions);
	  }
	  return true;
	});

      comp->force = gravity*comp->mass_data.mass;
      comp->update(obj, dt);
      
      if (counter % 2 == 0) {
	if (!comp->isMoving()) {
	  comp->onStop(obj);
	}
	comp->prev_veloc = comp->veloc;
      }

    }
  }
  
  void Physics::updateVsStatic(u32* num_checks, u32* num_collisions) {

    for (auto& obj : moving_objects) {

      auto& comp = obj->getComponent<DynamicPhysicsComp>();
      
      engine->traverseStatic<StaticPhysicsComp>
	(obj->getLooseBoundingObject(),
	 [&obj, &comp, &num_checks, &num_collisions]
	 (Pointer<StaticObject>& obj2,
	  Pointer<StaticPhysicsComp>& comp2) -> bool {
	  ++(*num_checks);
	  if (resolveCollision(obj, comp, obj2, comp2)) {
	    ++(*num_collisions);
	  }
	  return true;
	});
    }
  }
  
}
