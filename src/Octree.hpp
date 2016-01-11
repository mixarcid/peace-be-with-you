#pragma once

#include "Node.hpp"
#include "Function.hpp"

NAMESPACE {

  struct OctreeOutOfBoundsMessage : Message {

    bool should_insert;
    
    OctreeOutOfBoundsMessage()
      : should_insert(false) {
      $rttiConstruct("OctreeOutOfBoundsMessage");
    }
  };
  $registerRttiStruct();

  struct Octree {

    Vec3f center;
    Vec3f halves;
    Array<Node*> nodes;
    Octree* children[8];

    Octree(Vec3f _center, Vec3f _halves);
    ~Octree();
    void insert(Node* node, u32 level=0);
    void traverse(Node* node,
		  function<void(Node*,Node*)> callback);
    void traverseAll(function<void(Node*)> callback);

  };

}
