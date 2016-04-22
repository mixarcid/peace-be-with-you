#include "Grid.hpp"
#include "GameObject.hpp"
#include "VectorHash.hpp"

NAMESPACE {

  void Grid::insert(Pointer<DynamicObject>& obj) {
    if (obj->getLooseBoundingObject()->type == BoundingObject::NONE) {
      for (u8 n=0; n<4; ++n) {
	obj->handle.indexes[n] = -1;
      }
    } else {
      _insert(obj, _getCorner(obj));
    }
  }
  
  void Grid::update(Pointer<DynamicObject>& obj) {
    if (obj->handle.indexes[0] != -1) {
 
      Vec2i corner = _getCorner(obj);
    
      if (obj->handle.corner != corner) {
	_remove(obj, corner);
	_insert(obj, corner);
      }
    }
  }
  
  void Grid::remove(Pointer<DynamicObject>& obj) {
    if (obj->handle.indexes[0] != -1) {
      _remove(obj, _getCorner(obj));
    }
  }
  
  void Grid::traverse(BoundingObject* bound,
		      ContainerCallback<DynamicObject> callback) {
    for (auto& elem : objects) {
      auto& arr = elem.second;
      for (auto& obj : arr) {
	if (!callback(obj)) {
	  return;
	}
      }
    }
  }

  void Grid::traverseNeighbors(Pointer<DynamicObject>& obj,
			       ContainerCallback<DynamicObject> callback) {
    UnorderedSet<DynamicObject*> ptr_set;
    ptr_set.insert(obj);
    for (u8 n=0; n<4; ++n) {
      Vec2i cell(obj->handle.corner +
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

  Vec2i Grid::_getCorner(Pointer<DynamicObject>& obj) {
    Vec2f corner_f = obj->getTrans().xy()/CELL_SIZE;
    Vec2i corner = Vec2i(round(corner_f.x()-1),
			 round(corner_f.y())-1);
    return corner;
  }

  void Grid::_insert(Pointer<DynamicObject>& obj, Vec2i corner) {
    obj->handle.corner = corner;
    for (u8 n=0; n<4; ++n) {
      Vec2i cell(obj->handle.corner +
		 Vec2i(n & 1 ? 0 : 1,
		       n & 2 ? 0 : 1));
      auto f = objects.find(cell);
      if (f != objects.end()) {
	f->second.emplace_back(obj);
	obj->handle.indexes[n] = f->second.size() - 1;
      } else {
	auto it = objects.insert
	  (makePair
	   (cell,
	    Array<Pointer<DynamicObject>, u8>()));
	it.first->second.emplace_back(obj);
	obj->handle.indexes[n] = it.first->second.size() - 1;
      }
    }
  }

  void Grid::_remove(Pointer<DynamicObject>& obj, Vec2i corner) {
    
    for (u8 n=0; n<4; ++n) {
      
      Vec2i cell(obj->handle.corner +
		 Vec2i(n & 1 ? 0 : 1,
		       n & 2 ? 0 : 1));
	
      auto obj_it = objects.find(cell);
      debugAssert(obj_it != objects.end(),
		  "Something's gone wrong with the Grid... "
		  "Have you updated an object before inserting it?");
      auto& arr = obj_it->second;
	
      if (arr.size() == 1) {
	objects.erase(obj_it);
      } else {
	arr.removeAndReplace(arr.begin() + obj->handle.indexes[n]);
	if (obj->handle.indexes[n] < arr.size()) {

	  u8 index = obj->handle.indexes[n];
	  auto& elem = arr[index];
	  Vec2i diff = cell - elem->handle.corner;

	  debugAssert(diff.x() >= 0 && diff.x() <= 1 &&
		      diff.y() >= 0 && diff.y() <= 1,
		      "Something went wrong with Grid::_remove");
	  
	  u8 n = (diff.x() ? 0 : 1) + (diff.y() ? 0 : 2);
	  elem->handle.indexes[n] = index;
	  
	}
      }
    }
  }

}
