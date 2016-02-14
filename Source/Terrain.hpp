#pragma once

#include "Engine.hpp"
#include "StaticMesh.hpp"

NAMESPACE {

  struct TerrainChunk : GameObject {
    
    //pos: the back-left corner of the chunk. The z-value will always be 0.
    TerrainChunk(Engine* engine, Vec3f pos)
      : GameObject(engine, pos) {
      $rttiConstruct("TerrainChunk");
      init();
    }
    void init();
    
  };
  $registerRttiStruct();

  struct Terrain {

    Engine* engine;

    Terrain(Engine* _engine);
    //size in chunks
    void generate(Vec2u size);

    static Asset<Texture> texture;
    static Array<StaticMesh> chunk_meshes;
    static Array<u32> mesh_elems;
    static EBO elem_buffer;
    
  };

}
