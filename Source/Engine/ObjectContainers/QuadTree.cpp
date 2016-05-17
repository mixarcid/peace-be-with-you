#include "QuadTree.hpp"
#include "Engine.hpp"

NAMESPACE {

  QuadTree::Node::Node(BoundingAABB2D _bound)
    : bound(_bound) {
    for (u8 n=0; n<4; ++n) {
      children[n] = 0;
    }
  }

  void forceInsert(QuadTree::SizeType node,
		   QuadTree* tree,
		   Pointer<StaticObject>& obj,
		   u8 depth) {
    
    BoundingObject* obj_bound = obj->getLooseBoundingObject();
    BoundingAABB2D bound = tree->nodes[node].bound;
    
    Vec2f rel_center =
      (obj_bound->getCenter().xy() -
       bound.getCenter().xy());
      
    u8 child_index = 0;
    if (rel_center.x() > 0) {
      child_index += 1;
    }
    if (rel_center.y() > 0) {
      child_index += 2;
    }

    Vec2f child_halves = bound.halves/2;
    Vec2f center = bound.center;
    Vec2f child_offset
      (child_index & 1 ? child_halves.x() : -child_halves.x(),
       child_index & 2 ? child_halves.y() : -child_halves.y());
    BoundingAABB2D child_bound(center+child_offset,
			       child_halves);
    
    if (obj_bound->isContainedIn(&child_bound) && depth < tree->max_depth) {
      if (tree->nodes[node].children[child_index] == 0) {
	tree->nodes[node].children[child_index] = tree->nodes.size();
        tree->nodes.emplace_back(child_bound);
      }
      QuadTree::Node::insert
	(tree->nodes[node].children[child_index], tree, obj, depth+1);
    } else {
      tree->nodes[node].objects.emplace_back(obj);
    }
  }

  void QuadTree::Node::insert(SizeType node,
			      QuadTree* tree,
			      Pointer<StaticObject>& obj,
			      u8 depth) {
    u32 s = tree->nodes[node].objects.size();
    switch(s) {
    case 0:
      tree->nodes[node].objects.emplace_back(obj);
      break;
      /*case 1:
      {
	Pointer<StaticObject> tmp = tree->nodes[node].objects[0];
	tree->nodes[node].objects.clear();
	forceInsert(node, tree, tmp, depth);
	}*/
    default:
      forceInsert(node, tree, obj, depth);
      break;
    }
  }

  bool QuadTree::Node::traverse
    (SizeType node,
     QuadTree* tree,
     BoundingObject* obj_bound,
     ContainerCallback<StaticObject> callback) {

    for (auto& obj : tree->nodes[node].objects) {
      if (obj && !callback(obj)) {
	return false;
      }
    }
    
    for (u8 n=0; n<4; ++n) {
      SizeType child = tree->nodes[node].children[n];
      if (child != 0 &&
	  obj_bound->intersects(&tree->nodes[child].bound)) {
	//Log::message(to_string(child));
	if (!traverse(child, tree, obj_bound, callback)) {
	  return false;
	}
      }
    }
    return true;
  }

  QuadTree::QuadTree(BoundingAABB2D bound, u8 _max_depth)
    : max_depth(_max_depth) {
    nodes.emplace_back(bound);
  }
  
  void QuadTree::insert(Pointer<StaticObject>& obj) {
    if (obj->getLooseBoundingObject()->
	isContainedIn(&(nodes[0].bound))) {
      Node::insert(0, this, obj, 0);
    } else {
      OutOfBoundsMessage msg;
      obj->message(&msg);
      if (msg.should_insert) {
	nodes[0].objects.emplace_back(obj);
      }
    }
  }
  
  void QuadTree::traverse(BoundingObject* bound,
			  ContainerCallback<StaticObject> callback) {
    Node::traverse(0, this, bound, callback);
  }

  void QuadTree::clear() {
    BoundingAABB2D bound = nodes[0].bound;
    nodes.clear();
    nodes.emplace_back(bound);
  }

  String to_string(QuadTree& tree, QuadTree::Node& node, String delim) {
    
    String ret = "\n" + delim;
    ret += "`";
    ret += "center: " + to_string(node.bound.center) +
      ", #objects: " + to_string(node.objects.size());
    
    for (u8 n=0; n<4; ++n) {
      if (node.children[n] != 0) {
	String d = "  ";
	for (u8 i=1; i<4-n; ++i) {
	  if (node.children[n+i] != 0) {
	    d = " |";
	    break;
	  }
	}
	ret += to_string
	  (tree,
	   tree.nodes[node.children[n]],
	   delim + d);
      }
    }
    return ret;
  }
  
  String to_string(QuadTree& tree) {
    return "\n#nodes: " + to_string(tree.nodes.size()) + to_string(tree, tree.nodes[0], "");
  }
  
}
