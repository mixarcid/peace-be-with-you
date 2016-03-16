#pragma once

#include "ObjectContainer.hpp"
#include "Vector.hpp"
#include "BoundingFrustum.hpp"

NAMESPACE {

  template<>
    size_t customHash(Vec2i vec);

  struct Grid {

    struct Handle {
      Vec2i corner;
      u8 neighbors;
    };

    HashMap<Vec2i, Array<Pointer<GameObject>>> objects;
    const f32 CELL_SIZE;
    const f32 CELL_HALF;

    Grid(const f32 _CELL_SIZE)
      : CELL_SIZE(_CELL_SIZE),
	CELL_HALF(_CELL_SIZE/2) {}
    Handle insert(Pointer<GameObject>& obj);
    Handle update(Pointer<GameObject>& obj, Handle h);
    void remove(Pointer<GameObject>& obj, Handle h);
    //traversal ends when/if callback returns false
    void traverse(BoundingObject* bound,
		  ContainerCallback callback);
    void traverseNeighbors(Pointer<GameObject>& obj,
			   Handle h,
			   ContainerCallback callback);

    Vec2i _getCorner(Pointer<GameObject>& obj);

  };

}
