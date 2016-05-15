#pragma once

#include "Engine.hpp"
#include "TerrainRenderable.hpp"
#include "Tree.hpp"

NAMESPACE {

  struct TerrainChunkComp : Component {

    Array<Pointer<Tree>> trees;
    
    TerrainChunkComp() {
      $rttiConstruct("TerrainChunkComp");
    }
    
  };
  $registerRttiStruct();

  struct TerrainChunk : StaticObject {
    
    //pos: the back-left corner of the chunk. The z-value will always be 0.
    TerrainChunk(Vec3f pos)
      : StaticObject(pos) {
      $rttiConstruct("TerrainChunk");
      init();
    }
    void init();
    void removeTrees();
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

    struct ChunkLoadData {
      
      Pointer<TerrainChunk> chunk;
      Pointer<TerrainRenderable> mesh;
      Array<TreeData> tree_data;
      Vec2u index;

      void _on_move() {
	chunk._on_move();
	mesh._on_move();
      }
      
    };

    Array<Vec2u> erase_chunks;
    Array<ChunkLoadData> chunk_load_data;
    Mutex erase_chunk_mutex;
    Mutex chunk_load_mutex;

    Terrain();
    ~Terrain();
    //size in chunks
    void generate(String filename,
		  Vec3f pos,
		  Vec2u _size);
    void loadFile(String filename);
    void loadChunk(Vec2u index, bool cur_scene);
    Vec2u chunkAtPoint(Vec2f p);
    f32 heightAtPoint(Vec2f p, Vec3f* norm);

    void _scene_update(bool cur_scene);

    static Asset<Texture> texture;
    static Array<TerrainRenderable> chunk_meshes;
    static HashMap<Vec2u, Pointer<TerrainChunk>> chunks;
    static Mutex chunk_mesh_mutex;
    static Mutex chunk_mutex;
    static EBO elem_buffer_large;
    static EBO elem_buffer_mid;
    static EBO elem_buffer_small;

    static Array<Vec2f> pos_data;
    static VBO pos_buffer;

    static const f32 CHUNK_SIZE;
    static const u32 CHUNK_RES;
    static const f32 CHUNK_STEP;

    static const u32 CHUNKS_IN_VIEW;
    
  };

}
