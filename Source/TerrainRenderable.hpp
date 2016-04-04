#pragma once
#include "RenderableComp.hpp"
#include "Containers.hpp"
#include "Texture.hpp"

NAMESPACE {

  union BiomeData {
    
    struct {
      u8 grass_level;
      u8 rock_level;
      u8 snow_level;
      u8 sand_level;
    };
    Vec4ub levels;

    BiomeData() : levels() {}
    
  };

  struct TerrainRenderable : RenderableComp {

    Array<f32> height_data;
    Array<BiomeData> biome_data;
    Array<Vec3f> normal_data;
    VBO height_vbo;
    VBO biome_vbo;
    VBO normal_vbo;
    VAO vao;
    Texture tex;
    
    TerrainRenderable();
    void init();
    virtual void render(RenderContext c);
    
    const static ShaderFlags SHADER_FLAGS
    = SHADER_USE_NORMAL | SHADER_TERRAIN | SHADER_USE_TEXTURE | SHADER_3D;
    
  };

}
