#include "Quadtree.hpp"
#include "BoundingObject.hpp"

NAMESPACE {

  /*
    if n is our current index for child octrees:
    n % 2 < 1 -> positive x
    n % 4 < 2 -> positive y
    n < 4 -> positive z (we don't need the modulus)
  */

  QuadtreeNode::QuadtreeNode(Vec3f _center, Vec3f _halves)
    : box(_center, _halves) {
    for (u8 n=0; n<8; ++n) {
      children[n] = NULL;
    }
  }

  QuadtreeNode::~QuadtreeNode() {
    for (u8 n=0; n<4; ++n) {
      if (children[n]) {
	delete children[n];
      }
    }
  }
  
  void QuadtreeNode::insert(GameObject* node, u32 level) {

    BoundingObject bound = node->getTightBoundingObject();

    if (!bound.allInBox(box)) {
      QuadtreeOutOfBoundsMessage m;
      node->message(&m);
      if (m->should_insert) {
	nodes.push_back(node);
      }
      return;
    }
    
    if (nodes.size() == 0) {
      nodes.push_back(node);
      return;
    }

    Vec3f child_halves = Vec3f(box.halves.x()/2,
			       box.halves.y()/2,
			       box.halves.z());

    //Log::message("-----");
    for (u8 n=0; n<4; ++n) {
      
      Vec3f child_center =
	Vec3f(box.center.x() + ((n % 2 < 1) ? child_halves.x() : -child_halves.x()),
	      box.center.y() + ((n % 4 < 2) ? child_halves.y() : -child_halves.y()),
	      box.center.z());

      //Log::message("obb center: " + to_string(bound.obb.center));
      //Log::message("box center: " + to_string(child_center));
      //Log::message("box halves: " + to_string(child_halves));

      if (bound.allInBox(box)) {
	//Log::message("Yah!");
	if (children[n] == NULL) {
	  children[n] = new QuadtreeNode(child_center, child_halves);
	}
	children[n]->insert(node, level+1);
        return;
      }
    }
    nodes.push_back(node);
  }
  
  void QuadtreeNode::traverse(function<bool(BoundingAABB)> test,
			  function<void(GameObject*)> callback) {

    if (!test(box)) return
    
    for (GameObject* n : nodes) {
      callback(n);
    }

    BoundingObject bound = node->getLooseBoundingObject();

    for (u8 n=0; n<4; ++n) {
      if (children[n]) {
	  children[n]->traverse(callback);
      }
    }
  }
}
