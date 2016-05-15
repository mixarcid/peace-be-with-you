#pragma once
#include "Random.hpp"
#include "Noise.hpp"
#include "TerrainRenderable.hpp"
#include "Tree.hpp"

NAMESPACE {
  
  enum BiomeType {
    BIOME_TEST,
    BIOME_GRASSLAND,
    BIOME_JUNGLE,
    BIOME_MOUNTAIN,
    BIOME_DESERT,
    BIOME_SERENGETI,
    BIOME_OCEAN
  };

  struct TerrainGenerator;

  union BiomeDetailData {

    struct {
      f64 rockiness;
      f64 rock_amplitude;
      f64 rock_large_freq;
      f64 rock_small_freq;
    };
    Vec4d data;

    BiomeDetailData() : data() {}
    f64 detailAtPoint(Vec2d pos,
		      BiomeData* biome_data = NULL);
    
  };

  struct BiomeCenter {

    TerrainGenerator* gen;
    Vec2d center;
    Vec2d dir;
    f64 height;
    f64 amplitude;
    f64 freq;
    u32 rand_seed;
    BiomeType biome;
    Random rand;
    FrankNoise noise;
    BiomeDetailData detail;

    BiomeCenter(TerrainGenerator* _gen,
		Vec2d _center,
		Vec2d _dir,
		f64 _height,
		f64 _amplitude,
		f64 _freq,
		u32 _rand_seed,
		BiomeType _biome)
      : gen(_gen),
	center(_center),
	dir(_dir),
	height(_height),
	amplitude(_amplitude),
	freq(_freq),
	rand_seed(_rand_seed),
	biome(_biome) {}

    f64 dataAtPoint(Vec2d old_pos,
		    Vec2d pos,
		    BiomeData* biome_data = NULL);
    bool treeAtPoint(Vec2d pos, TreeType* type);
    
  };

  struct TreeGrid {
    
    Vec2d center;
    Vec2u res;
    Vec2d size;
    Vec2d step;

    TreeGrid(Vec2u _res, Vec2d _size)
      : res(_res),
	size(_size),
	step(Vec2d(size.x()/res.x(), size.y()/res.y())) {}
  };

  struct Terrain;

  struct TerrainGenerator {

    Terrain* ter;
    Array<BiomeCenter> biome_centers;
    BiomeCenter* closest_biome_centers[4];
    
    Vec3d pos;
    Vec2d size;
    Vec2u num_biomes;
    Vec2d step;
    Vec2d internal_step;
    Vec2d pos_offset;

    Vec2i prev_grid_pos;
    TreeGrid tree_grid;
    Array<TreeData> chunk_trees;
    
    FrankNoise noise[5];
    
    TerrainGenerator(Terrain* _ter,
		     Vec3d _pos,
		     Vec2d _size,
		     Vec2u _num_biomes,
		     Vec2d _chunk_size,
		     Vec2d _internal_step);
    void setChunkCenter(Vec2d chunk_center);
    f64 dataAtPoint(Vec2d point,
		    BiomeData* biome_data = NULL);
    Array<TreeData> getChunkTrees();

    static Random global_random;
    
    static const u8 NUM_BIOME_TYPES;
    static const f64 SEA_FLOOR_HEIGHT;
    static const f64 SEA_LEVEL;
    static const f64 SNOW_TOP_LEVEL;
    static const f64 SNOW_BOTTOM_LEVEL;
    static const f64 SNOW_LEVEL_DIFF;
    static const f64 MOUNTAIN_HEIGHT_MIN;
    static const f64 MOUNTAIN_HEIGHT_DIFF;
  };

}
