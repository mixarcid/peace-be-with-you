#pragma once

#include "GL.hpp"
#include "Standard.hpp"
#include "String.hpp"
#include "Shader.hpp"

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
