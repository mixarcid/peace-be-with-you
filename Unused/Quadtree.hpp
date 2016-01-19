#pragma once

#include "Function.hpp"
#include "Vector.hpp"
#include "Containers.hpp"

NAMESPACE {

  struct GameObject;

  struct QuadtreeOutOfBoundsMessage : Message {

    bool should_insert;
    
    QuadtreeOutOfBoundsMessage()
      : should_insert(false) {
      $rttiConstruct("QuadtreeOutOfBoundsMessage");
    }
  };
  $registerRttiStruct();

  struct QuadtreeNode {
    
    BoundingAABB box;
    Array<GameObject*> nodes;
    QuadtreeNode* children[4];

    QuadtreeNode(Vec3f _center, Vec3f _halves);
    ~QuadtreeNode();
    void insert(GameObject* node, u32 level);
    void traverse(function<bool(BoundingAABB)> test,
		  function<void(GameObject*)> callback);

  };

  struct Quadtree {

    Array<QuadtreeNode> nodes;

    Quadtree(Vec3f _center, Vec3f _halves, u32 init_size);
    ~Quadtree();
    void insert(GameObject* node);
    void traverse(function<bool(BoundingAABB)> test,
		  function<void(GameObject*)> callback);
    
  };

}
