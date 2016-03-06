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

    for (DynamicComponentPairP<DynamicPhysicsComp> obj : moving_objects) {

      engine->traverseStatic<StaticPhysicsComp>
	(obj.obj->getLooseBoundingObject(),
	 [obj, &num_checks, &num_collisions](StaticComponentPair<StaticPhysicsComp> obj2) -> bool {
	  ++num_checks;
	  if (resolveCollision(obj, obj2)) {
	    ++num_collisions;
	  }
	  return true;
	});
      engine->traverseDynamic<DynamicPhysicsComp>
	(obj.obj->getLooseBoundingObject(),
	 [obj, &num_checks, &num_collisions](DynamicComponentPair<DynamicPhysicsComp> obj2) -> bool {
	  ++num_checks;
	  if (resolveCollision(obj, obj2)) {
	    ++num_collisions;
	  }
	  return true;
	});

      obj.comp->force = gravity*obj.comp->mass_data.mass;
      //Log::message(to_string(obj.comp->veloc));
      obj.comp->update(obj.obj, dt);
      
      if (counter % 2 == 0) {
	if (!obj.comp->isMoving()) {
	  obj.comp->onStop(obj.obj);
	}
	obj.comp->prev_veloc = obj.comp->veloc;
      }

    }
    Log::message("collision checks: %u", num_checks);
    Log::message("collisions: %u", num_collisions);
  }
  
}
