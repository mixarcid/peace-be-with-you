#pragma once

#include "Node.hpp"
#include "Function.hpp"

NAMESPACE {

  struct Octree {

    Vec3f center;
    Vec3f halves;
    Array<Node*> nodes;
    Octree* children[8];

    Octree(Vec3f _center, Vec3f _halves);
    ~Octree();
    void insert(Node* node);
    void traverse(Node* node,
		  function<void(Node*,Node*)> callback);

  };

}
