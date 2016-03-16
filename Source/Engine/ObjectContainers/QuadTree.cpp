#if 0
#include "QuadTree.hpp"
#include "Engine.hpp"

NAMESPACE {

  QuadTree::Node::Node(BoundingAABB2D _bound,
		       SizeType _parent)
    : bound(_bound), parent(_parent) {
    for (u8 n=0; n<4; ++n) {
      children[n] = -1;
    }
  }

  Handle QuadTree::Node::addObject(SizeType index) {
    Handle ret;
    ret.node_index = index;
    ret.obj_index = objects.size();
    objects.push_back(obj);
    return ret;
  }

  bool QuadTree::Node::traverse(QuadTree* tree,
				BoundingObject* obj_bound,
				function<bool
				(Pointer<GameObject>)> callback) {

    for (auto& obj : objects) {
      if (!callback(obj)) {
	return false;
      }
    }
    
    for (u8 n=0; n<4; ++n) {
      if (n != -1 &&
	  obj_bound->intersects(tree->nodes[children[n]].bound) &&
	  !traverse(tree, obj_bound, callback) {
	    return false;
      }
    }
    return true;
  }

  QuadTree::QuadTree(BoundingAABB2D bound)
    : root(bound, -1) {}
  
  Handle QuadTree::insert(Pointer<GameObject> obj) {

    SizeType index = -1;
    
    while(true) {
      Node* node = _getNode(index);
      index = _insertHelper(node, obj);
      if (index == -1) {
        return node->addObject(obj);
      }
    }
  }
      
  /*Handle QuadTree::update(Handle handle) {

    remove(handle);
    Node* node = nodes[handle.node_index];
    Pointer<GameObject> obj = node->objects[handle.obj_index];
    BoundingObject* obj_bound =
      obj->getLooseBoundingObject();
   
    if (obj_bound.isContainedIn(node->bound)) {
      
      while(true) {
	Node* node = _getNode(index);
	index = _insertHelper(node, obj);
	if (index == -1) {
	  return node->addObject(obj);
	}
      }
      
    } else {
      
      SizeType index = node->parent;
      
      while (true) {
	node = _getNode(index);
	if (obj_bound.isContainedIn(node->bound)) {
	  return node->addObject(obj);
	} else if (index == -1) {
	  OutOfBoundsMessage m;
	  obj->message(&m);
	  if (m.should_insert) {
	    return node->addObject(obj);
	  }
	}
      }
    }
    return ret;
  }

  void QuadTree::remove(Handle handle) {
    nodes[handle.node_index].objects.removeAndReplace
      (&(nodes[node_index].objects[handle.obj_index]));
      }*/
  
  void QuadTree::traverse(BoundingObject* bound,
			  function<bool(Pointer<GameObject>)> callback) {
    root.traverse(this, bound, callback);
  }

  void QuadTree::_getNode(SizeType index) {
    if (index == -1) {
      return root;
    } else {
      return nodes[index];
    }
  }

  u8 QuadTree::_getChildIndex(BoundingObject* bound) {
    
    Vec2f rel_center =
      (node->bound.getCenter().xy() +
       bound.getCenter().xy());
      
    u8 child_index = 0;
    if (rel_center.x() > 0) {
      child_index += 1;
    }
    if (rel_center.y() > 0) {
      child_index += 2;
    }
    return child_index;
  }

  SizeType QuadTree::_insertHelper(Node* node,
				   Pointer<GameObject> obj) {
    u8 child_index = _getChildIndex(obj->getLooseBoundingObject());  
    if (obj_bound.isContainedIn(node->bound)) {
      if (node->children[child_index] == 0) {
	  
	Vec2f child_halves = node->bound.halves/2;
	Vec2f center = node->bound.center;
	Vec2f child_offset
	  (child_index & 1 ? child_halves.x() : -child_halves.x(),
	   child_index & 2 ? child_halves.y() : -child_halves.y());
	BoundingAABB2D bound(center+child_offset,
			     child_halves);
	node->children[child_index] = nodes.emplace_back(bound, index);
	  
      }
      index = node->children[child_index];
      return index;
    } else {
      return -1;
    }
  }
}
#endif
