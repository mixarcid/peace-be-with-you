#pragma once

#include "GL.hpp"
#include "Standard.hpp"
#include "String.hpp"
#include "GLObject.hpp"
#include "Assets.hpp"

NAMESPACE {

  struct Texture : GLObject {

    Texture() {}
    ~Texture();
    
    void init();
    void loadFromFile(String filename);
    void createEmpty(Vec2i size);
    void use();
  };

  template<>
    Texture* loadAsset<Texture>(String name);
}
