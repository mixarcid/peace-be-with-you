#include "Grid.hpp"
#include "GameObject.hpp"

NAMESPACE {

  template<>
    size_t customHash(Vec2i vec) {
    return ((Hash<u32>()(vec.x())) ^
	    (Hash<u32>()(vec.y()) << 1));
  }

  Grid::Handle Grid::insert(Pointer<GameObject>& obj) {
    
    Vec2f halves(CELL_HALF, CELL_HALF);
    Handle ret;
    ret.corner = _getCorner(obj);
    ret.neighbors = 0;
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
    remove(obj, h);
    return insert(obj);
  }
  
  void Grid::remove(Pointer<GameObject>& obj, Handle h) {

    Log::message("---");
    for (auto& elem : objects) {
      auto& arr = elem.second;
      Log::message(to_string(elem.first));
      for (auto& o : arr) {
	if (obj == o) {
	  Log::message("  %u, %p", o.index, (Pointer<Pointable>*)&o);
	}
      }
    }
    
    for (u8 n=0; n<4; ++n) {
      
      if (h.neighbors & (1 << n)) {
	
	Vec2i cell(h.corner +
		   Vec2i(n & 1 ? 0 : 1,
			 n & 2 ? 0 : 1));
	Log::message(to_string(cell));
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
	      Log::message("data now: %u, %p", it->index, it);
	      arr.removeAndReplace(it);
	      for (auto& p : elem->pointers) {
		Log::message("now: %p", p);
	      }
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
    //Log::message("neighbors: %u", h.neighbors);
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
	  if (!callback(elem)) {
	    return;
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
