#pragma once
#include "Random.hpp"
#include "Noise.hpp"
#include "TerrainRenderable.hpp"
#include "Tree.hpp"

NAMESPACE {
  
  enum BiomeType {
    BIOME_GRASSLAND,
    BIOME_JUNGLE,
    BIOME_MOUNTAIN,
    BIOME_DESERT,
    BIOME_OCEAN
  };

  struct TerrainGenerator;

  struct BiomeCenter {

    TerrainGenerator* gen;
    Vec2f center;
    f32 height;
    u32 rand_seed;
    BiomeType biome;
    Random rand;
    FrankNoise noise;

    BiomeCenter(TerrainGenerator* _gen,
		Vec2f _center,
		f32 _height,
		u32 _rand_seed,
		BiomeType _biome)
      : gen(_gen),
	center(_center),
	height(_height),
	rand_seed(_rand_seed),
	biome(_biome) {}

    f32 dataAtPoint(Vec2f old_pos,
		    Vec2f pos,
		    BiomeData* biome_data = NULL);
    bool treeAtPoint(Vec2f pos, TreeType* type);
    
  };

  struct TreeGrid {
    
    //completely inefficient to have an Array of bools,
    //but it's darn simple to impliment and it shouldn't
    //be a memory bottleneck at all
    //Array<bool> trees;
    Vec2f center;
    Vec2u res;
    Vec2f size;
    Vec2f step;

    TreeGrid(Vec2u _res, Vec2f _size)
      : res(_res),
	size(_size),
	step(Vec2f(size.x()/res.x(), size.y()/res.y())) {}
  };

  struct Terrain;

  struct TerrainGenerator {

    Terrain* ter;
    Array<BiomeCenter> biome_centers;
    BiomeCenter* closest_biome_centers[4];
    
    Vec3f pos;
    Vec2f size;
    Vec2u num_biomes;
    Vec2f step;
    Vec2f internal_step;
    Vec2f pos_offset;

    Vec2i prev_grid_pos;
    TreeGrid tree_grid;
    Array<TreeData> chunk_trees;
    
    FrankNoise noise[5];
    
    TerrainGenerator(Terrain* _ter,
		     Vec3f _pos,
		     Vec2f _size,
		     Vec2u _num_biomes,
		     Vec2f _chunk_size,
		     Vec2f _internal_step);
    void setChunkCenter(Vec2f chunk_center);
    f32 dataAtPoint(Vec2f point,
		    BiomeData* biome_data = NULL);
    Array<TreeData> getChunkTrees();

    static Random global_random;
    
    static const u8 NUM_BIOME_TYPES;
    static const f32 SEA_FLOOR_HEIGHT;
    static const f32 SEA_LEVEL;
    static const f32 SNOW_TOP_LEVEL;
    static const f32 SNOW_BOTTOM_LEVEL;
    static const f32 SNOW_LEVEL_DIFF;
  };

}
