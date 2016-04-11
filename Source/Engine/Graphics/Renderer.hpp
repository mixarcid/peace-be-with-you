#pragma once

#include "Shader.hpp"
#include "FBO.hpp"
#include "ScreenQuad.hpp"
#include "Texture.hpp"

NAMESPACE {

  struct Renderer {

    Shader first_shade;
    Shader second_shade;
    Shader third_shade;
    
    ScreenQuad screen_quad;
    ShaderVar screen_coord;
    
    ShaderUniform diffuse_uniform;
    ShaderUniform normal_uniform;
    ShaderUniform offset_uniform;
    ShaderUniform depth_uniform;

    ShaderUniform smooth_uniform;
    ShaderUniform pass2_uniform;

    FBO g_buffer;
    
    Texture diffuse;
    Texture normal;
    Texture offset;
    Texture depth;
    Texture paint;

    Texture smooth_normal;
    Texture pass2;

    Vec2i window_size;

    Renderer();
    void init(Vec2i win_size);
    void onWindowResize(Vec2i win_size);
    //call before rendering 3d objects
    void prepare();
    //call after
    void finalize();

  };

}
