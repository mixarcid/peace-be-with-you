#pragma once

#include "GL.hpp"
#include "Standard.hpp"
#include "String.hpp"
#include "Shader.hpp"
#include "Assets.hpp"

NAMESPACE {

  struct Texture {

    u32 length;
    u32* ids;

    Texture(u32 num_textures = 1);
    void init();
    void load(String filename,
	      ShaderUniform tex_uniform,
	      u32 index = 0);
    void use(u32 index = 0);
    ~Texture();
  };

  template<>
    Texture* loadAsset<Texture>(String name);
}
