#pragma once

#include "Standard.h"

#define DIR_SEP "/"

#define DIR_ASSETS "Assets" DIR_SEP
#define DIR_SHADERS DIR_ASSETS "Shaders" DIR_SEP
#define DIR_TEXTURES DIR_ASSETS "Textures" DIR_SEP
#define DIR_SHADER_HEADER DIR_SHADERS "Header3d"

NAMESPACE {
  void logCurrentDirectory();
}
