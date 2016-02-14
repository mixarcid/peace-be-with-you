#pragma once

#include "BoundingObject.hpp"

NAMESPACE {

  struct Manifold {

    Vec3f normal;
    f32 penetration;
    u32 num_contacts;
    Vec3f contacts[8];
    
  };

}
