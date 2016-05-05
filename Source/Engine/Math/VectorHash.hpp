#pragma once

#include "Vector.hpp"
#include "Containers.hpp"

namespace std {

  template<>
  struct hash<peace::Vec2i> {
    std::size_t operator()(const peace::Vec2i& key) const {
      return ((hash<peace::u32>()(key.cx())
	       ^ (hash<peace::u32>()(key.cy()) << 1)) >> 1);
    }
  };

  template<>
  struct hash<peace::Vec2u> {
    std::size_t operator()(const peace::Vec2u& key) const {
      return ((hash<peace::u32>()(key.cx())
	       ^ (hash<peace::u32>()(key.cy()) << 1)) >> 1);
    }
  };

}

/*NAMESPACE {

  struct Vec2iHash {
    std::size_t operator()(const Vec2i& key) const {
      return ((hash<u32>()(key.x())
	       ^ (hash<u32>()(key.y()) << 1)) >> 1);
    }
  };

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
  
	    }*/
