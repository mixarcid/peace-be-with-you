#pragma once
#include "Noise.hpp"
#include "TerrainRenderable.hpp"

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
    
  };

  struct TerrainGenerator {
    
    Array<BiomeCenter> biome_centers;
    BiomeCenter* closest_biome_centers[4];
    Vec3f pos;
    Vec2f size;
    Vec2u num_biomes;
    Vec2f step;
    Vec2f pos_offset;
    FrankNoise noise[5];
    
    TerrainGenerator(Vec3f _pos,
		     Vec2f _size,
		     Vec2u _num_biomes);
    void setChunkCenter(Vec2f chunk_center);
    f32 dataAtPoint(Vec2f point,
		    BiomeData* biome_data = NULL);
    void generateFoilage(Vec2f min,
			 Vec2f max,
			 Array<Vec3f>* grass_positions);

    static const u8 NUM_BIOME_TYPES;
    static const f32 SEA_FLOOR_HEIGHT;
  };

}
