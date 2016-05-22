#pragma once

#include "PhysicsComp.hpp"

NAMESPACE {

  struct CollisionData {
    GameObject::IdSize a_id;
    GameObject::IdSize b_id;
  };
  
  bool operator==(CollisionData a, CollisionData b);
  
}

namespace std {

  template<>
  struct hash<peace::CollisionData> {
    std::size_t operator()(const peace::CollisionData& key) const {
      return ((hash<peace::GameObject::IdSize>()(key.a_id)
	       + (hash<peace::GameObject::IdSize>()(key.b_id))));
    }
  };
}

NAMESPACE {

  struct CollisionMessage : Message {

    //the object the messagee collided with
    Pointer<GameObject> object;

    CollisionMessage(Pointer<GameObject> _object)
      : object(_object) {
      $rttiConstruct("CollisionMessage");
    }
  };
  $registerRttiStruct();

  struct Engine;
  
  struct Physics {

    Engine* engine;
    
    UnorderedSet<CollisionData> collision_set[2];
    bool collision_set_first;
		 
    DynamicPhysicsComp::MovingObjectArray moving_objects;
    Vec3f gravity;

    Physics(Engine* _engine);

    //both functions add the total number of checks and collisions
    //to the input values
    //updateVsDynamic also finalizes everything, so it should
    //be called last
    void updateVsStatic(u32* num_checks, u32* num_collisions);
    void updateVsDynamic(u32* num_checks, u32* num_collisions);

    UnorderedSet<CollisionData>* getCollisionSet(bool cur_set);
    void swapCollisionSets();
    
  };

}
