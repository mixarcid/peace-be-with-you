#pragma once

#include "Pointable.hpp"
#include "BoundingAABB2D.hpp"
#include "ObjectContainer.hpp"

NAMESPACE {
  
  struct QuadTree {

    typedef i32 SizeType;

    struct Node {

      Array<Pointer<StaticObject>> objects;
      BoundingAABB2D bound;
      SizeType children[4];

      Node(BoundingAABB2D _bound);
      static void insert(SizeType node,
			 QuadTree* tree,
			 Pointer<StaticObject>& obj,
			 u8 depth);
      static bool traverse(SizeType node,
			   QuadTree* tree,
			   BoundingObject* obj_bound,
			   ContainerCallback<StaticObject> callback);
       
    };

    Array<Node> nodes;
    u8 max_depth;

    QuadTree(BoundingAABB2D bound, u8 _max_depth);

    void insert(Pointer<StaticObject>& obj);
    void traverse(BoundingObject* bound,
		  ContainerCallback<StaticObject> callback);
    void clear();

  };

  String to_string(QuadTree& tree, QuadTree::Node& node, String delim);
  String to_string(QuadTree& tree);

}
