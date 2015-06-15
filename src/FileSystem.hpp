#pragma once

#include "Standard.hpp"

#define DIR_SEP "/"

#define DIR_ASSETS "Assets" DIR_SEP
#define DIR_SHADERS DIR_ASSETS "Shaders" DIR_SEP
#define DIR_TEXTURES DIR_ASSETS "Textures" DIR_SEP
#define DIR_SHADER_HEADER DIR_SHADERS "Header3d"
#define DIR_MODELS DIR_ASSETS "Models" DIR_SEP

#define DIR_VERT_EXTENSION ".vs"
#define DIR_FRAG_EXTENSION ".fs"
#define DIR_MODEL_EXTENSION ".pmf"
#define DIR_TEXTURE_EXTENSION ".png"

NAMESPACE {
  void LogCurrentDirectory();
}
