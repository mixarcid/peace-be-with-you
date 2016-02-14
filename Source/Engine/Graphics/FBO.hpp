#pragma once

#include "GL.hpp"
#include "Standard.hpp"
#include "Texture.hpp"
#include "Vector.hpp"

NAMESPACE {

  PEACE_DEFINE_BITFLAGS(FBOFlags, 8,
			FBO_NO_FLAGS = 0x00,
			FBO_USE_DEPTH_BUFFER = 0x01);

  struct FBO : GLObject {

    u32 depth_buffer;
    u8 num_targets;

    FBO();
    ~FBO();
    
    void init();
    void addDepthBuffer(Vec2i size);
    void bindTargets(Array<Texture> textures);
    void clearTargets(Array<Vec4f> colors);
    void use();
    bool isComplete();

    //after rendering to FBO, render to screen
    static void useScreen();
    
  };

}
