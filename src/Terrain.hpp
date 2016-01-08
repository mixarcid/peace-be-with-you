#pragma once

#include "Engine.hpp"
#include "StaticMesh.hpp"

NAMESPACE {

  struct TerrainChunk : GameObject {

    StaticMesh mesh;

    //pos: the back-left corner of the chunk. The z-value will always be 0.
    TerrainChunk(Vec3f pos, Engine* engine);
    virtual BoundingObject getBoundingObject();
    virtual BoundingObject getPhysicalBoundingObject();
    virtual BoundingObject getPrimaryBoundingObject();

    static Array<u32> mesh_elems;
    
  };

  struct Terrain {

    Engine* engine;
    Array<TerrainChunk> chunks;

    Terrain(Engine* _engine);
    //size in chunks
    void generate(Vec2<u32> size);

    static Texture* TEXTURE;
  };

}
