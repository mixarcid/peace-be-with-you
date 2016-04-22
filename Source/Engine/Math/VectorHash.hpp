#pragma once

#include "Vector.hpp"
#include "Containers.hpp"

NAMESPACE {

  template<>
    size_t customHash(Vec2i vec) {
    return ((Hash<u32>()(vec.x())) ^
	    (Hash<u32>()(vec.y()) << 1));
  }

  template<>
    size_t customHash(Vec2u vec) {
    return ((Hash<u32>()(vec.x())) ^
	    (Hash<u32>()(vec.y()) << 1));
  }
  
}
