#pragma once

#include "ObjectContainer.hpp"
#include "Vector.hpp"
#include "BoundingFrustum.hpp"
#include "VectorHash.hpp"

NAMESPACE {

  struct Grid {

    struct Handle {
      
      Vec2i corner;
      i8 indexes[4];

      Handle() : corner(0,0), indexes{-1,-1,-1,-1} {}
    };

    HashMap<Vec2i, Array<Pointer<DynamicObject>, u8>> objects;
    const f32 CELL_SIZE;
    const f32 CELL_HALF;

    Grid(const f32 _CELL_SIZE)
      : CELL_SIZE(_CELL_SIZE),
	CELL_HALF(_CELL_SIZE/2) {}
    void insert(Pointer<DynamicObject>& obj);
    void update(Pointer<DynamicObject>& obj);
    void remove(Pointer<DynamicObject>& obj);
    //traversal ends when/if callback returns false
    void traverse(BoundingObject* bound,
		  ContainerCallback<DynamicObject> callback);
    void traverseNeighbors(Pointer<DynamicObject>& obj,
			   ContainerCallback<DynamicObject> callback);

    inline Vec2i _getCorner(Pointer<DynamicObject>& obj);
    inline void _insert(Pointer<DynamicObject>& obj, Vec2i corner);
    inline void _remove(Pointer<DynamicObject>& obj, Vec2i corner);

  };

}
