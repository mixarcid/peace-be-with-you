#include "Octree.hpp"
#include "BoundingObject.hpp"

NAMESPACE {

  /*
    if n is our current index for child octrees:
    n % 2 < 1 -> positive x
    n % 4 < 2 -> positive y
    n < 4 -> positive z (we don't need the modulus)
  */

  Octree::Octree(Vec3f _center, Vec3f _halves)
    : center(_center), halves(_halves) {
    for (u8 n=0; n<8; ++n) {
      children[n] = NULL;
    }
  }

  Octree::~Octree() {
    for (u8 n=0; n<8; ++n) {
      if (children[n]) {
	delete children[n];
      }
    }
  }
  
  void Octree::insert(Node* node, u32 level) {

    BoundingObject bound = node->getTightBoundingObject();

    if (!bound.allInBox(center, halves)) {
      OctreeOutOfBoundsMessage m;
      node->message(&m);
      return;
    }
    
    if (nodes.size() == 0) {
      nodes.push_back(node);
      return;
    }

    Vec3f child_halves = halves/2;

    //Log::message("-----");
    for (u8 n=0; n<8; ++n) {
      
      Vec3f child_center =
	Vec3f(center.x() + ((n % 2 < 1) ? child_halves.x() : -child_halves.x()),
	      center.y() + ((n % 4 < 2) ? child_halves.y() : -child_halves.y()),
	      center.z() + ((n < 4) ? child_halves.z() : -child_halves.z()));

      //Log::message("obb center: " + to_string(bound.obb.center));
      //Log::message("box center: " + to_string(child_center));
      //Log::message("box halves: " + to_string(child_halves));

      if (bound.allInBox(child_center, child_halves)) {
	//Log::message("Yah!");
	if (children[n] == NULL) {
	  children[n] = new Octree(child_center, child_halves);
	}
	children[n]->insert(node, level+1);
        return;
      }
    }
    nodes.push_back(node);
  }
  
  void Octree::traverse(Node* node,
			function<void(Node*,Node*)> callback) {
    
    for (Node* n : nodes) {
      callback(node, n);
    }

    BoundingObject bound = node->getLooseBoundingObject();

    for (u8 n=0; n<8; ++n) {
      if (children[n]) {
	if (bound.someInBox(children[n]->center,
			    children[n]->halves)) {
	  children[n]->traverse(node, callback);
	} else {
	}
      }
    }
  }

  void Octree::traverseAll(function<void(Node*)> callback) {
    for (Node* n : nodes) {
      callback(n);
    }

    for (u8 n=0; n<8; ++n) {
      if (children[n]) {
	children[n]->traverseAll(callback);
      }
    }
	
  }

}
