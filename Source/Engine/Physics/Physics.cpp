#include "Physics.hpp"
#include "Collision.hpp"
#include "Engine.hpp"

NAMESPACE {

  bool operator==(CollisionData a, CollisionData b) {
    return (((a.a_id == b.a_id) && (a.b_id == b.b_id)) ||
	    ((a.b_id == b.a_id) && (a.a_id == b.b_id)));
  }

  Physics::Physics(Engine* _engine)
    : engine(_engine), gravity(Vec3f(0,0,-30)) {}
  
  void Physics::updateVsStatic(u32* num_checks, u32* num_collisions) {

    for (auto& obj : moving_objects) {

      auto& comp = obj->getComponent<DynamicPhysicsComp>();
      engine->traverseStatic<StaticPhysicsComp>
	(obj->getLooseBoundingObject(),
	 [&obj, &comp, &num_checks, &num_collisions, this]
	 (Pointer<StaticObject>& obj2,
	  Pointer<StaticPhysicsComp>& comp2) -> bool {
	  
	  ++(*num_checks);
	  if (resolveCollision(obj, comp, obj2, comp2)) {

	    UnorderedSet<CollisionData>* cur_set = getCollisionSet(true);
	    UnorderedSet<CollisionData>* last_set = getCollisionSet(false);
		  
	    CollisionData cd;
	    cd.a_id = obj->id;
	    cd.b_id = obj2->id;
	    
	    cur_set->insert(cd);
	    ++(*num_collisions);

	    if (last_set->find(cd) == last_set->end()) {
	      CollisionMessage a_msg((Pointer<GameObject>&)obj);
	      CollisionMessage b_msg((Pointer<GameObject>&)obj2);
	      obj->message(&b_msg);
	      obj2->message(&a_msg);
	    }
	    
	  }
	  
	  return true;
	});
    }
  }

  void Physics::updateVsDynamic(u32* num_checks, u32* num_collisions) {

    static u32 counter = 0;
    counter = counter > 7 ? 0 : counter + 1;

    const f32 dt = engine->dt;

    for (auto& obj : moving_objects) {

      auto& comp = obj->getComponent<DynamicPhysicsComp>();
      
      engine->traverseNeighbors<DynamicPhysicsComp>
	(obj,
	 [&obj, &comp, num_checks, num_collisions, this]
	 (Pointer<DynamicObject>& obj2,
	  Pointer<DynamicPhysicsComp>& comp2) -> bool {
	  
	  ++(*num_checks);
	  if (resolveCollision(obj, comp, obj2, comp2)) {

	    UnorderedSet<CollisionData>* cur_set = getCollisionSet(true);
	    UnorderedSet<CollisionData>* last_set = getCollisionSet(false);
		  
	    CollisionData cd;
	    cd.a_id = obj->id;
	    cd.b_id = obj2->id;
	    
	    cur_set->insert(cd);
	    ++(*num_collisions);

	    if (last_set->find(cd) == last_set->end()) {
	      CollisionMessage a_msg((Pointer<GameObject>&)obj);
	      CollisionMessage b_msg((Pointer<GameObject>&)obj2);
	      obj->message(&b_msg);
	      obj2->message(&a_msg);
	    }
	    
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
    swapCollisionSets();
  }

  UnorderedSet<CollisionData>* Physics::getCollisionSet(bool cur_set) {
    UnorderedSet<CollisionData>* ret;
    if (collision_set_first) {
      ret = &collision_set[cur_set];
    } else {
      ret = &collision_set[!cur_set];
    }
    return ret;
  }

  void Physics::swapCollisionSets() {
    collision_set_first = !collision_set_first;
    getCollisionSet(true)->clear();
  }
  
}
