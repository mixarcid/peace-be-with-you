#pragma once
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

  struct BiomeCenter {

    Vec2f center;
    BiomeType biome;
    FrankNoise noise;

    BiomeCenter(Vec2f _center, BiomeType _biome)
      : center(_center), biome(_biome) {}

    f32 dataAtPoint(Vec2f pos,
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

  struct TerrainGenerator {
    
    Array<BiomeCenter> biome_centers;
    BiomeCenter* closest_biome_centers[4];
    
    Vec3f pos;
    Vec2f size;
    Vec2u num_biomes;
    Vec2f step;
    Vec2f internal_step;
    Vec2f pos_offset;

    TreeGrid tree_grid;
    Array<TreeData> chunk_trees;
    
    FrankNoise noise[5];
    
    TerrainGenerator(Vec3f _pos,
		     Vec2f _size,
		     Vec2u _num_biomes,
		     Vec2f _internal_step);
    void setChunkCenter(Vec2f chunk_center);
    f32 dataAtPoint(Vec2f point,
		    BiomeData* biome_data = NULL);
    Array<TreeData> getChunkTrees();

    static const u8 NUM_BIOME_TYPES;
    static const f32 SEA_FLOOR_HEIGHT;
    static const f32 SEA_LEVEL;
  };

}
