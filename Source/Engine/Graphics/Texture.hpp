#pragma once

#include "GL.hpp"
#include "Standard.hpp"
#include "String.hpp"
#include "GLObject.hpp"
#include "Assets.hpp"

NAMESPACE {

  PEACE_DEFINE_BITFLAGS(TextureFlags, 8,
			TEXTURE_NO_FLAGS,
			TEXTURE_NO_GAMMA);

  struct Texture : GLObject {

    Texture() {}
    ~Texture();
    
    void init();
    void loadFromFile(String filename,
		      TextureFlags flags = TEXTURE_NO_FLAGS,
		      GLenum format = GL_RGBA);
    void createEmpty(Vec2i size, GLenum format = GL_RGB);
    void use();
  };

  template<>
    Texture* loadAsset<Texture>(String name);
}
