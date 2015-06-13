#pragma once

#include "Graphics.hpp"
#include "Standard.hpp"
#include "String.hpp"
#include "Shader.hpp"

NAMESPACE {

  const unsigned int DEFAULT_NUM_TEXTURES = 500;

  struct Texture {

    unsigned int index;
    const ShaderUniform* uniform;

    //Texture(Texture* tex);
    void use();
    void load(String filename, const ShaderUniform* tex_uniform);

    static unsigned int length;
    static GLuint* ids;
    static bool is_initialized;
    static unsigned int current;

    static void init(unsigned int num_textures
		     = DEFAULT_NUM_TEXTURES);
    static Texture getTexture();
    static void terminate();
  };

}
