#pragma once

#include "Shader.hpp"

NAMESPACE {

  struct Graphics : Graphics3d, Graphics2d {
    Shader shade;
    Graphics(String shader_name);
  }
  
}
