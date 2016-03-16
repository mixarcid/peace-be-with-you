#include "Grid.hpp"
#include "GameObject.hpp"

NAMESPACE {

  template<>
    size_t customHash(Vec2i vec) {
    return ((Hash<u32>()(vec.x())) ^
	    (Hash<u32>()(vec.y()) << 1));
  }

  Grid::Handle Grid::insert(Pointer<GameObject>& obj) {
    
    Handle ret;
    ret.neighbors = 0;

    if (obj->getLooseBoundingObject()->type ==  BoundingObject::NONE) {
      return ret;
    }

    Vec2f halves(CELL_HALF, CELL_HALF);
    ret.corner = _getCorner(obj);
    //Log::message("-------");
    for (u8 n=0; n<4; ++n) {
      Vec2i cell(ret.corner +
		 Vec2i(n & 1 ? 0 : 1,
		       n & 2 ? 0 : 1));
      BoundingAABB2D aabb
	(Vec2f(cell)*CELL_SIZE + halves, halves);
      /* Log::message("Cell insert: " +
		   to_string(obj->getTrans().xy()) +
		   to_string(aabb.center));*/
      if (aabb.intersects(obj->getLooseBoundingObject())) {
	//Log::message("insert");
	ret.neighbors |= (1 << n);
	auto f = objects.find(cell);
	if (f != objects.end()) {
	  f->second.emplace_back(obj);
	} else {
	  auto it = objects.insert
	    (makePair
	     (cell,
	      Array<Pointer<GameObject>>()));
	  it.first->second.emplace_back(obj);
	}
      }
    }
    return ret;
  }
  
  Grid::Handle Grid::update(Pointer<GameObject>& obj, Handle h) {

    if (!h.neighbors) {
      return h;
    }
    
    Handle ret;
    ret.corner = _getCorner(obj);
    ret.neighbors = 0;
    
    if (h.corner == ret.corner) {

      Vec2f halves(CELL_HALF, CELL_HALF);
      
      for (u8 n=0; n<4; ++n) {
	
	Vec2i cell(ret.corner +
		   Vec2i(n & 1 ? 0 : 1,
			 n & 2 ? 0 : 1));
	BoundingAABB2D aabb
	  (Vec2f(cell)*CELL_SIZE + halves, halves);
	bool intersects = aabb.intersects(obj->getLooseBoundingObject());

	if (intersects) {
	  ret.neighbors |= (1 << n);
	}
	
	if (intersects && !(h.neighbors & (1 << n))) {
	  
	  auto f = objects.find(cell);
	  if (f != objects.end()) {
	    f->second.emplace_back(obj);
	  } else {
	    auto it = objects.insert
	      (makePair
	       (cell,
		Array<Pointer<GameObject>>()));
	    it.first->second.emplace_back(obj);
	  }
	  
	} else if (!intersects && (h.neighbors & (1 << n))) {
	  
	  auto obj_it = objects.find(cell);
	  debugAssert(obj_it != objects.end(),
		      "Something's gone wrong with the Grid...");
	  auto& arr = obj_it->second;
	
	  if (arr.size() == 1) {
	    objects.erase(obj_it);
	  } else {
	    for (auto it = arr.begin(); it != arr.end(); ++it) {
	      auto& elem = *it;
	      if (obj == elem) {
		arr.removeAndReplace(it);
		goto end_for;
	      }
	    }
	    Log::fatalError("Grid::update should never reach this point");
	  end_for: ;
	  
	  }
	}
	
      }
    } else {
      remove(obj, h);
      ret = insert(obj);
    }
    return ret;
  }
  
  void Grid::remove(Pointer<GameObject>& obj, Handle h) {

    if (!h.neighbors) {
      return;
    }
    
    for (u8 n=0; n<4; ++n) {
      
      if (h.neighbors & (1 << n)) {
	
	Vec2i cell(h.corner +
		   Vec2i(n & 1 ? 0 : 1,
			 n & 2 ? 0 : 1));
	
	auto obj_it = objects.find(cell);
	debugAssert(obj_it != objects.end(),
		    "Something's gone wrong with the Grid...");
	auto& arr = obj_it->second;
	
	if (arr.size() == 1) {
	  objects.erase(obj_it);
	} else {
	  
	  for (auto it = arr.begin(); it != arr.end(); ++it) {
	    auto& elem = *it;
	    if (obj == elem) {
	      arr.removeAndReplace(it);
	      goto end_for;
	    }
	  }
	  Log::fatalError("Grid::remove should never reach this point");
	end_for: ;
	}
	
      }
    }
  }
  
  void Grid::traverse(BoundingObject* bound,
		      ContainerCallback callback) {
    for (auto& elem : objects) {
      auto& arr = elem.second;
      for (auto& obj : arr) {
	if (!callback(obj)) {
	  return;
	}
      }
    }
  }

  void Grid::traverseNeighbors(Pointer<GameObject>& obj,
			       Handle h,
			       ContainerCallback callback) {
    UnorderedSet<GameObject*> ptr_set;
    ptr_set.insert(obj);
    for (u8 n=0; n<4; ++n) {
      if (h.neighbors & (1 << n)) {
	Vec2i cell(h.corner +
		   Vec2i(n & 1 ? 0 : 1,
			 n & 2 ? 0 : 1));
	auto obj_it = objects.find(cell);
	debugAssert(obj_it != objects.end(),
		    "Something's gone wrong with the Grid...");
	auto& arr = obj_it->second;
	for (auto& elem : arr) {
	  if (ptr_set.find(elem) == ptr_set.end()) {
	    if (!callback(elem)) {
	      return;
	    }
	    ptr_set.insert(elem);
	  }
	}
      }
    }
  }

  Vec2i Grid::_getCorner(Pointer<GameObject>& obj) {
    Vec2f corner_f = obj->getTrans().xy()/CELL_SIZE;
    Vec2i corner = Vec2i(round(corner_f.x()-1),
			 round(corner_f.y())-1);
    //Log::message(to_string(corner));
    return corner;
  }

}
