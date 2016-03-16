/*#pragma once

#include "Pointable.hpp"
#include "BoundingAABB2D.hpp"
#include "ObjectContainer.hpp"

NAMESPACE {

  struct GameObject;
  
  struct QuadTree {

    typedef i32 SizeType;

    struct Handle {
      SizeType node_index;
      Array<Pointer<GameObject>>::SizeType obj_index;
    };

    struct Node {

      Array<Pointer<GameObject>> objects;
      BoundingAABB2D bound;
      SizeType parent;
      SizeType children[4];

      Node(BonedAABB2D _bound, SizeType _parent);
      Handle addObject(SizeType index);
      bool traverse(QuadTree* tree,
		    BoundingObject* obj_bound,
		    function<bool(Pointer<GameObject>)> callback);
       
    };

    Array<Node> nodes;
    Node root;

    QuadTree(BoundingAABB2D bound);
    
    Handle insert(Pointer<GameObject> obj);
    //Handle update(Handle handle);
    //void remove(Handle handle);
    //traversal ends when/if callback returns false
    void traverse(BoundingObject* bound,
		  function<bool(Pointer<GameObject>)> callback);

    Node* _getNode(SizeType index);
    u8 _getChildIndex(BoundingObject* bound);
    SizeType _insertHelper(Node* node,
			   Pointer<GameObject> obj);

  };

  }*/
