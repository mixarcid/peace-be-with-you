#pragma once

#include "StaticMesh.hpp"

NAMESPACE {

  struct Terrain : StaticMesh {
    void generate(float width, float height);
  };

}
