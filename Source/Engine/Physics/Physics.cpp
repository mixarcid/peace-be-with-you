#include "Physics.hpp"
#include "Collision.hpp"
#include "Engine.hpp"

NAMESPACE {

  Physics::Physics(Engine* _engine)
    : engine(_engine), gravity(Vec3f(0,0,-9.8)) {}

  void Physics::update() {

    static u32 counter = 0;
    counter = counter > 7 ? 0 : counter + 1;

    const f32 dt = engine->dt;
    u32 num_checks = 0;
    u32 num_collisions = 0;

    for (auto& obj : moving_objects) {

      auto& comp = obj->getComponent<DynamicPhysicsComp>();
      
      engine->traverseStatic<StaticPhysicsComp>
	(obj->getLooseBoundingObject(),
	 [&obj, &comp, &num_checks, &num_collisions]
	 (Pointer<StaticObject>& obj2,
	  Pointer<StaticPhysicsComp>& comp2) -> bool {
	  ++num_checks;
	  if (resolveCollision(obj, comp, obj2, comp2)) {
	    ++num_collisions;
	  }
	  return true;
	});
      engine->traverseNeighbors<DynamicPhysicsComp>
	(obj,
	 [&obj, &comp, &num_checks, &num_collisions]
	 (Pointer<DynamicObject>& obj2,
	  Pointer<DynamicPhysicsComp>& comp2) -> bool {
	  ++num_checks;
	  if (resolveCollision(obj, comp, obj2, comp2)) {
	    ++num_collisions;
	  }
	  return true;
	});

      comp->force = gravity*comp->mass_data.mass;
      //Log::message(to_string(obj.comp->veloc));
      comp->update(obj, dt);
      
      if (counter % 2 == 0) {
	if (!comp->isMoving()) {
	  comp->onStop(obj);
	}
	comp->prev_veloc = comp->veloc;
      }

    }
    Log::message("collision checks: %u", num_checks);
    Log::message("collisions: %u", num_collisions);
  }
  
}
