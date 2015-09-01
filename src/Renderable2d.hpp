#pragma once

#include "Containers.hpp"
#include "Vector.hpp"

NAMESPACE {

  struct Renderable2dData {
    Vec3f pos;
    Vec2f tex_coord;
  };

  struct Renderable2d {

    Array<Renderable2dData> data;
    
  };

}
