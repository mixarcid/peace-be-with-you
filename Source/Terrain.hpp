#pragma once

#include "Engine.hpp"
#include "TerrainRenderable.hpp"

NAMESPACE {

  struct TerrainChunk : GameObject {
    
    //pos: the back-left corner of the chunk. The z-value will always be 0.
    TerrainChunk(Engine* engine, Vec3f pos)
      : GameObject(engine, pos) {
      $rttiConstruct("TerrainChunk");
      init();
    }
    void init();
    ~TerrainChunk();
    
  };
  $registerRttiStruct();

  struct Terrain {

    Engine* engine;
    Pointer<GameObject> ground_object;

    Terrain(Engine* _engine);
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
    
  };

}
