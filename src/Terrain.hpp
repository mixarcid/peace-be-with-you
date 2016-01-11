#pragma once

#include "Engine.hpp"
#include "TerrainMesh.hpp"

NAMESPACE {

  struct TerrainChunk : GameObject {

    TerrainMesh mesh;

    BoundingObject getTightBoundingObject();
    BoundingObject getLooseBoundingObject();
    
    //pos: the back-left corner of the chunk. The z-value will always be 0.
    TerrainChunk(Vec3f pos);
    void init(Engine* engine);
    
  };

  struct Terrain {

    Engine* engine;
    Array<TerrainChunk> chunks;

    Terrain(Engine* _engine);
    //size in chunks
    void generate(Vec2u size);

    static Texture* TEXTURE;
    static Array<u32> MESH_ELEMS;
    static EBO ELEM_BUFFER;
  };

}
