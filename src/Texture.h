#pragma once

#include "Graphics.h"
#include "Standard.h"
#include "String.h"
#include "Shader.h"

NAMESPACE {

  struct Texture {

    unsigned int length;
    GLuint* ids;

    Texture(unsigned int num_textures = 1);
    void load(String filename, ShaderUniform tex_uniform,
	      unsigned int index = 0);
    void use(unsigned int index = 0);
    ~Texture();
  };

}
