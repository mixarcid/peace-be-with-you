#include "TerrainGenerator.hpp"
#include "Interp.hpp"
#include "Time.hpp"

NAMESPACE {

  const u8 TerrainGenerator::NUM_BIOME_TYPES = 5;
  const f32 TerrainGenerator::SEA_FLOOR_HEIGHT = -10;
  
  f32 BiomeCenter::dataAtPoint(Vec2f pos,
			       BiomeData* biome_data) {
    switch(biome) {
      
    case BIOME_OCEAN:

      if (biome_data) {
	biome_data->sand_level = 255;
      }
      return TerrainGenerator::SEA_FLOOR_HEIGHT + 5*noise.getValue(pos/30);

    case BIOME_GRASSLAND:

      if (biome_data) {
	biome_data->grass_level = 255;
      }
      return 20 + 20*noise.getValue(pos/100);

    case BIOME_JUNGLE:
    case BIOME_MOUNTAIN:
    case BIOME_DESERT:
      return 0;
    }
  }

  //const f32 INTERP_FACTOR = 10;
  inline f32 interpFunc(f32 val) {
    f32 ret = 3*val*val - 2*val*val*val;
    /*f32 ret =  0.5 + atan((val - 0.5)*INTERP_FACTOR)
     *(1/M_PI*atan(INTERP_FACTOR*0.5));*/
    return ret;
  }

  inline Vec2f interpFunc(Vec2f val) {
    return Vec2f(interpFunc(val.x()),
		 interpFunc(val.y()));
  }
  
  TerrainGenerator::TerrainGenerator(Vec3f _pos,
				     Vec2f _size,
				     Vec2u _num_biomes)
    : pos(_pos),
    size(_size),
    num_biomes(_num_biomes),
    step(size.x()/num_biomes.x(),
	 size.y()/num_biomes.y()),
    pos_offset(pos.xy() -
	       (Vec2f((num_biomes.x()-1)*step.x(),
		      (num_biomes.y()-1)*step.y())/2)) {
      
      //maximum radius for the island
      f32 max_radius = (size.x()/2)*0.5;
      Time t;
      t.makeCurrent();
      srand(t.getMilliseconds());

      biome_centers.reserve((num_biomes.x()+4)*(num_biomes.y()+4));

      //create grid specifying biome centers
      for (i32 x = -2; x < (i32)num_biomes.x() + 2; ++x) {
	for (i32 y = -2; y < (i32)num_biomes.y() + 2; ++y) {
	
	  Vec2f center = pos_offset +
	    Vec2f(step.x()*x, step.y()*y);
	  
	  Log::message(to_string(center));
	  Log::message(to_string(center.norm()));
	  BiomeType biome;
	  if ((center - pos.xy()).norm() > max_radius) {
	    biome = BIOME_OCEAN;
	    Log::message("Ocean");
	  } else {
	    biome = BIOME_GRASSLAND;
	    Log::message("Grassland");
	  }

	  biome_centers.emplace_back(center, biome);
	
	}
      }
    }

  void TerrainGenerator::setChunkCenter(Vec2f chunk_center) {}
  
  f32 TerrainGenerator::dataAtPoint(Vec2f point,
				    BiomeData* biome_data) {

    Vec2f input = Vec2f(point.x()/(2*step.x()),
			point.y()/(2*step.y()));
    Vec2f point_offset
      (noise[0].getValue(input)*step.x(),
       noise[1].getValue(input)*step.y());
    Vec2f new_point = point + point_offset;
    Vec2f rel_pos = new_point - pos_offset + size/2;
    //I honestly have no idea
    rel_pos -= Vec2f(step.x()*(num_biomes.x()/2.0-2),
		     step.y()*(num_biomes.y()/2.0-2));
    
    Vec2u indexes(floor(rel_pos.x()/step.x()),
		  floor(rel_pos.y()/step.y()));
    Vec2f rel(rel_pos.x() - indexes.x()*step.x(),
	      rel_pos.y() - indexes.y()*step.y());
    u32 x_mul = num_biomes.x() + 4;
    
    closest_biome_centers[0] = &biome_centers
      [indexes.x()*x_mul + indexes.y()];
    closest_biome_centers[2] = &biome_centers
      [(indexes.x()+1)*x_mul + indexes.y()];
    closest_biome_centers[1] = &biome_centers
      [indexes.x()*x_mul + indexes.y() + 1];
    closest_biome_centers[3] = &biome_centers
      [(indexes.x()+1)*x_mul + indexes.y() + 1];

    Vec2f rem = new_point - closest_biome_centers[0]->center;
    rem.x() /= step.x();
    rem.y() /= step.y();
    rem = Vec2f(1.0,1.0) - rem;

    f32 heights[4];
    Vec4ub biome_array[4];
    
    for (u8 n=0; n<4; ++n) {
      BiomeData b;
      heights[n] = closest_biome_centers[n]
	->dataAtPoint(point, biome_data ? &b : NULL);
      biome_array[n] = b.levels;
    }

    Vec2f trans_noise = Vec2f(1 + noise[3].getValue(new_point/200),
			      1 + noise[4].getValue(new_point/200))*0.5;

    if (rem.x() > 1.0) rem.x() -= 1.0;
    if (rem.y() > 1.0) rem.y() -= 1.0;
    
    if (indexes.x() % 2 == 0) {
      trans_noise.x() = 1 - trans_noise.x();
    }
    if (indexes.y() % 2 == 0) {
      trans_noise.y() = 1 - trans_noise.y();
    }

    Vec2f noise_factor(1-2*abs(rem.x()-0.5),
		       1-2*abs(rem.y()-0.5));
    noise_factor = interpFunc(noise_factor);
    noise_factor *= 0.7;
    
    rem = interpFunc(rem);
    rem.x() = (1-noise_factor.x())*rem.x() +
      noise_factor.x()*trans_noise.x();
    rem.y() = (1-noise_factor.y())*rem.y() +
    noise_factor.y()*trans_noise.y();

    rem.x() = rem.x() < 0.0 ? 0.0 : rem.x();
    rem.x() = rem.x() > 1.0 ? 1.0 : rem.x();
    rem.y() = rem.y() < 0.0 ? 0.0 : rem.y();
    rem.y() = rem.y() > 1.0 ? 1.0 : rem.y();
    
    f32 ret = biLerp(rem, heights);
    if (biome_data) {
      biome_data->levels = biLerp(rem, biome_array);
    }
    return ret;
  }

  void generateFoilage(Vec2f min, Vec2f max, Array<Vec3f>* grass_positions) {

  }
}
