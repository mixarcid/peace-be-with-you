#include "Physics.hpp"
#include "Collision.hpp"

NAMESPACE {

  Physics::Physics(Vec3f center, Vec3f halves)
    : static_objects(center, halves), gravity(0,0,-9.8067) {}

  void Physics::update(f32 dt) {
    for (PhysicalObject* a : dynamic_objects) {
      a->force = gravity*a->mass_data.mass;
      a->update(dt);
    }
    u32 n = 0;
    for (PhysicalObject* a : dynamic_objects) {
      static_objects.traverse(a, [&n](Node* a, Node* b) {
	  ++n;
	  resolveCollision((PhysicalObject*) a,
			   (PhysicalObject*) b);
	});
      for (PhysicalObject* b : dynamic_objects) {
	++n;
	resolveCollision(a,b);
      }
    }
    Log::message("%u", n);
  }

  void Physics::addStaticObject(StaticObject* obj) {
    static_objects.insert(obj);
  }
  
  u32 Physics::addDynamicObject(PhysicalObject* obj) {
    dynamic_objects.push_back(obj);
    return dynamic_objects.size() - 1;
  }

}
