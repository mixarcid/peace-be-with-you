#include "Physics.hpp"
#include "Collision.hpp"
#include "Engine.hpp"

NAMESPACE {

  Physics::Physics(Engine* _engine)
    : engine(_engine), gravity(Vec3f(0,0,-9.8)) {}

  void Physics::update() {

    const f32 dt = engine->dt;

    engine->traverseDynamic<PhysicsComp>
      (NULL, [this, dt](ComponentPair<PhysicsComp> obj) {

	engine->traverseStatic<PhysicsComp>
	  (obj.obj->getLooseBoundingObject(),
	   [obj](ComponentPair<PhysicsComp> obj2) {
	    resolveCollision(obj, obj2, true);
	  });
	engine->traverseDynamic<PhysicsComp>
	  (obj.obj->getLooseBoundingObject(),
	   [obj](ComponentPair<PhysicsComp> obj2) {
	    resolveCollision(obj, obj2, false);
	  });

	//Log::message(to_string(obj.obj->getTrans()));
	obj.comp->force = gravity*obj.comp->mass_data.mass;
	obj.comp->update(dt);
	obj.obj->transRel(obj.comp->veloc*dt);

      }); 
  }
  
}
