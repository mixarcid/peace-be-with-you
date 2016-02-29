#include "Physics.hpp"
#include "Collision.hpp"
#include "Engine.hpp"

NAMESPACE {

  Physics::Physics(Engine* _engine)
    : engine(_engine), gravity(Vec3f(0,0,-9.8)) {}

  void Physics::update() {

    const f32 dt = engine->dt;
    u32 num_checks = 0;
    u32 num_collisions = 0;

    engine->traverseDynamic<PhysicsComp>
      (NULL, [this, dt, &num_checks, &num_collisions](ComponentPair<PhysicsComp> obj) {

	engine->traverseStatic<PhysicsComp>
	  (obj.obj->getLooseBoundingObject(),
	   [obj, &num_checks, &num_collisions](ComponentPair<PhysicsComp> obj2) {
	    ++num_checks;
	    if (resolveCollision(obj, obj2, true)) {
	      ++num_collisions;
	    }
	  });
	engine->traverseDynamic<PhysicsComp>
	  (obj.obj->getLooseBoundingObject(),
	   [obj, &num_checks, &num_collisions](ComponentPair<PhysicsComp> obj2) {
	    ++num_checks;
	    if (resolveCollision(obj, obj2, false)) {
	      ++num_collisions;
	    }
	  });

	obj.comp->force = gravity*obj.comp->mass_data.mass;
	obj.comp->update(dt);
	obj.obj->transRel(obj.comp->veloc*dt);

      });
    Log::message("collision checks: %u", num_checks);
    Log::message("collisions: %u", num_collisions);
  }
  
}
