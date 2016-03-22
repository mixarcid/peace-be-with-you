#include "QuadTree.hpp"
#include "Engine.hpp"

NAMESPACE {

  QuadTree::Node::Node(BoundingAABB2D _bound)
    : bound(_bound) {
    for (u8 n=0; n<4; ++n) {
      children[n] = 0;
    }
  }

  void QuadTree::Node::_force_insert(QuadTree* tree,
				     Pointer<StaticObject>& obj,
				     u8 depth) {
    BoundingObject* obj_bound = obj->getLooseBoundingObject();
    
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
    
    if (obj_bound->isContainedIn(&child_bound) && depth < tree->MAX_DEPTH) {
      if (children[child_index] == 0) {
	children[child_index] = tree->nodes.size();
        tree->nodes.emplace_back(child_bound);
      }
      tree->nodes[children[child_index]].insert(tree, obj, depth+1);
    } else {
      objects.emplace_back(obj);
    }
  }

  void QuadTree::Node::insert(QuadTree* tree,
			      Pointer<StaticObject>& obj,
			      u8 depth) {
    u32 s = objects.size();
    switch(s) {
    case 0:
      objects.emplace_back(obj);
      break;
      /*case 1:
      {
	Pointer<StaticObject> tmp = objects[0];
	objects.clear();
	_force_insert(tree, tmp, depth);
	}*/
    default:
      _force_insert(tree, obj, depth);
      break;
    }
  }

  bool QuadTree::Node::traverse
    (QuadTree* tree,
     BoundingObject* obj_bound,
     ContainerCallback<StaticObject> callback) {

    for (auto& obj : objects) {
      if (!callback(obj)) {
	return false;
      }
    }
    
    for (u8 n=0; n<4; ++n) {
      if (children[n] != 0 &&
	  obj_bound->intersects(&tree->nodes[children[n]].bound) &&
	  !tree->nodes[children[n]].traverse(tree, obj_bound, callback)) {
	return false;
      }
    }
    return true;
  }

  QuadTree::QuadTree(BoundingAABB2D bound, const u8 _MAX_DEPTH)
    : MAX_DEPTH(_MAX_DEPTH) {
    nodes.emplace_back(bound);
  }
  
  void QuadTree::insert(Pointer<StaticObject>& obj) {
    nodes[0].insert(this, obj, 0);
  }
  
  void QuadTree::traverse(BoundingObject* bound,
			  ContainerCallback<StaticObject> callback) {
    nodes[0].traverse(this, bound, callback);
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
