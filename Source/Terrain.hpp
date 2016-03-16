#pragma once

#include "Engine.hpp"
#include "TerrainRenderable.hpp"

NAMESPACE {

  struct TerrainChunk : StaticObject {
    
    //pos: the back-left corner of the chunk. The z-value will always be 0.
    TerrainChunk(Vec3f pos)
      : StaticObject(pos) {
      $rttiConstruct("TerrainChunk");
      init();
    }
    void init();
    ~TerrainChunk();
    
  };
  $registerRttiStruct();

  struct Terrain {

    Pointer<StaticObject> ground_object;

    Terrain();
    ~Terrain();
    //size in chunks
    void generate(Vec3f pos, Vec2u size);

    static Asset<Texture> texture;
    static Array<TerrainRenderable> chunk_meshes;
    static EBO elem_buffer_large;
    static EBO elem_buffer_mid;
    static EBO elem_buffer_small;

    static const f32 CHUNK_SIZE;
    static const u32 CHUNK_RES;
    static const f32 CHUNK_STEP;
    static const StaticMaterial material;
    
  };

}
