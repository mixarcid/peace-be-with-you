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

  struct Terrain;

  struct TerrainGround : StaticObject {

    TerrainGround(Vec3f pos, Terrain* ter)
      : StaticObject(pos) {
      $rttiConstruct("TerrainGround");
      init(ter);
    }
    void init(Terrain* ter);
    ~TerrainGround();
    virtual void message(Message* msg);

    static const StaticMaterial MATERIAL;
    
  };
  $registerRttiStruct();
  

  struct Terrain {

    Pointer<TerrainGround> ground_object;
    FILE* file;
    Vec2u size;
    Vec3f chunk_pos_offset;
    u64 chunk_table_pos;

    Terrain();
    ~Terrain();
    //size in chunks
    void generate(String filename,
		  Vec3f pos,
		  Vec2u _size);
    void loadFile(String filename);
    void loadChunk(Vec2u index);

    static Asset<Texture> texture;
    static Array<TerrainRenderable> chunk_meshes;
    static EBO elem_buffer_large;
    static EBO elem_buffer_mid;
    static EBO elem_buffer_small;

    static Array<Vec2f> pos_data;
    static VBO pos_buffer;

    static const f32 CHUNK_SIZE;
    static const u32 CHUNK_RES;
    static const f32 CHUNK_STEP;
    
  };

}
