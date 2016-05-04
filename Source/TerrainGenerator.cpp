#include "TerrainGenerator.hpp"
#include "Terrain.hpp"
#include "Interp.hpp"
#include "Time.hpp"

NAMESPACE {

  Random TerrainGenerator::global_random;
  const u8 TerrainGenerator::NUM_BIOME_TYPES = 5;
  const f32 TerrainGenerator::SEA_FLOOR_HEIGHT = -10;
  const f32 TerrainGenerator::SEA_LEVEL = 0;
  const f32 TerrainGenerator::SNOW_TOP_LEVEL = 450;
  const f32 TerrainGenerator::SNOW_BOTTOM_LEVEL = 400;
  const f32 TerrainGenerator::SNOW_LEVEL_DIFF = SNOW_TOP_LEVEL - SNOW_BOTTOM_LEVEL;
  
  inline f32 interpFunc(f32 t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
  }

  inline Vec2f interpFunc(Vec2f val) {
    return Vec2f(interpFunc(val.x()),
		 interpFunc(val.y()));
  }

  inline f32 grassFunc(BiomeCenter* c, Vec2f pos) {
    //return c->noise.getValue(pos/100);
    return Noise::fractal
      ([c](Vec2f in, i32 index) -> f32 {
	return c->noise.getValue(in, index);
      }, pos/200, 2);
  }

  Vec2f PERLIN_OFFSET = Vec2f(1000, 2000);
  
  f32 BiomeCenter::dataAtPoint(Vec2f old_pos,
			       Vec2f pos,
			       BiomeData* biome_data) {

    f32 ret = 0;
    rand.seed(rand_seed);
    
    switch(biome) {
      
    case BIOME_OCEAN:

      if (biome_data) {
	biome_data->sand_level = 255;
      }
      ret += height + 5*noise.getValue(old_pos/30);
      break;

    case BIOME_GRASSLAND:

      if (biome_data) {
	biome_data->grass_level = 255;
      }
      ret += height + 20*grassFunc(this, old_pos);
      break;

    case BIOME_JUNGLE:
      
      break;
      
    case BIOME_MOUNTAIN:
      {
	f32 val = Noise::fractal
	  ([this](Vec2f in, i32 index) -> f32 {
	    f32 val = noise.getValue(in, index);
	    if (abs(val) < 0.1) {
	      val = val*val/0.1;
	    } else {
	      val = abs(val);
	    }
	    return 0.5 - val;
	  }, old_pos/1000, 7, 2.0, 0.5);

	Vec2f center_dist = (pos - center);
	center_dist.x() = 0.5*abs(2*1.414*center_dist.x()/(gen->step.x()));
	center_dist.y() = 0.5*abs(2*1.414*center_dist.y()/(gen->step.y()));
	center_dist = Vec2f(1,1) - center_dist;
	//Log::message(to_string(center_dist));
	
        ret += 75*val + (center_dist.norm())*height;
	//Log::message(to_string(ret));
	if (biome_data) {
	  biome_data->rock_level = 255;
	}
      }
      break;
      
    case BIOME_DESERT:

      Vec2f offset = Vec2f
	(noise.getValue(old_pos/5000, 0),
	 noise.getValue(old_pos/5000, 1))*20;
      f32 mag = 0.5 + 0.5*noise.getValue(old_pos/100, 2);
      ret += height + 30*(0.5 + mag)*noise.getValue(old_pos/2000 + offset, 3);
      if (biome_data) {
	biome_data->sand_level = 255;
      }
      break;
    }
    return ret;
  }

  bool BiomeCenter::treeAtPoint(Vec2f pos, TreeType* type) {

    bool ret;
    switch(biome) {

    case BIOME_GRASSLAND:
      {
        ret = true;
	f32 val =  Noise::fractal
	  ([this](Vec2f in, i32 index) -> f32 {
	    return noise.getValue(in);
	  }, pos/100 + Vec2f(900, 600), 5);
	if (val > 0.7) {
	  *type = TREE_WILLOW;
	} else if (val > 0.5) {
	  *type = TREE_PINE;
	} else if (val > 0.3) {
	  *type = TREE_ELM;
	} else {
	  ret = false;
	}
	break;
      }

    default:
      ret = false;
      break;
      
    }
    
    return ret;
  }

  struct BiomeCenterData {
    Vec2f center;
    f32 height;
    BiomeType biome;
  };
  
  TerrainGenerator::TerrainGenerator(Terrain* _ter,
				     Vec3f _pos,
				     Vec2f _size,
				     Vec2u _num_biomes,
				     Vec2f _chunk_size,
				     Vec2f _internal_step)
    : ter(_ter),
    pos(_pos),
    size(_size),
    num_biomes(_num_biomes),
    step(size.x()/num_biomes.x(),
	 size.y()/num_biomes.y()),
    internal_step(_internal_step),
    pos_offset(pos.xy() -
	       (Vec2f((num_biomes.x()-1)*step.x(),
		      (num_biomes.y()-1)*step.y())/2)),
    tree_grid(Vec2u(_chunk_size/30), _chunk_size) {
      
    //maximum radius for the island
    f32 max_radius = (size.x()/2)*0.9;
    Time t;
    t.makeCurrent();
    u32 seed = t.getMilliseconds();
    global_random.seed(seed);

    u32 size = (num_biomes.x()+4)*(num_biomes.y()+4);
    Array<BiomeCenterData> biome_data(size);
    biome_centers.reserve(size);
      
    //create grid specifying biome centers
    for (i32 x = -2; x < (i32)num_biomes.x() + 2; ++x) {
      for (i32 y = -2; y < (i32)num_biomes.y() + 2; ++y) {

	BiomeCenterData d;
        d.center = pos_offset +
	  Vec2f(step.x()*x, step.y()*y);
        d.height = 0;
	
	if ((d.center - pos.xy()).norm() > max_radius) {
	  d.biome = BIOME_OCEAN;
	  d.height = SEA_FLOOR_HEIGHT + (global_random()/(f32)global_random.max() - 0.5)*5;
	} else if ((d.center - pos.xy()).norm() >
		   max_radius/2) {
	  /*d.biome = BIOME_GRASSLAND;
	    d.height = SEA_LEVEL + (global_random()/(f32)global_random.max())*10 + 10;*/
	  d.biome = BIOME_DESERT;
	  d.height = (global_random()/(f32)global_random.max())*20 + 40;
	} else {
	  d.biome = BIOME_MOUNTAIN;
	  //Log::message("mountain");
	  d.height = SEA_LEVEL + 30 + (global_random()/(f32)global_random.max())*500;
	  //Log::message(to_string(d.height));
	}
	
	biome_data.push_back(d);
      }
    }

    for (BiomeCenterData d : biome_data) {
      biome_centers.emplace_back(this, d.center, d.height, ++seed, d.biome);
    }
    
  }

  void TerrainGenerator::setChunkCenter(Vec2f chunk_center) {
    prev_grid_pos = Vec2i(-1,-1);
    tree_grid.center = chunk_center;
    chunk_trees.clear();
  }
  
  f32 TerrainGenerator::dataAtPoint(Vec2f point,
				    BiomeData* biome_data) {

    Vec2f input = Vec2f(point.x()/(5*step.x()),
			point.y()/(5*step.y()));
    Vec2f point_offset
    (noise[0].getValue(input)*step.x()/2,
      noise[1].getValue(input)*step.y()/2);
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
	->dataAtPoint(new_point, new_point, biome_data ? &b : NULL);
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

    /*Vec2f noise_factor(1-2*abs(rem.x()-0.5),
		       1-2*abs(rem.y()-0.5));
		       noise_factor *= 0.5;*/
    
    rem = interpFunc(rem);
    /*rem.x() = (1-noise_factor.x())*rem.x() +
      noise_factor.x()*trans_noise.x();
    rem.y() = (1-noise_factor.y())*rem.y() +
    noise_factor.y()*trans_noise.y();*/
      rem = interpFunc(rem);

    rem.x() = rem.x() < 0.0 ? 0.0 : rem.x();
    rem.x() = rem.x() > 1.0 ? 1.0 : rem.x();
    rem.y() = rem.y() < 0.0 ? 0.0 : rem.y();
    rem.y() = rem.y() > 1.0 ? 1.0 : rem.y();
    
    f32 ret = biLerp(rem, heights);
    if (biome_data) {

      Vec2f rel_pos_grid = point - tree_grid.center + tree_grid.size/2;
      Vec2i grid_pos(rel_pos_grid.x()/tree_grid.step.x(),
		     rel_pos_grid.y()/tree_grid.step.y());
      //Log::message(to_string(grid_pos.x()) + to_string(rel_pos_grid) + to_string(internal_step));
      if ((grid_pos.x() > prev_grid_pos.x() ||
	   grid_pos.y() > prev_grid_pos.y()) &&
	  grid_pos.x() < (i32) tree_grid.res.x() -1 &&
	  grid_pos.y() < (i32) tree_grid.res.y() -1) {

	prev_grid_pos = grid_pos;
	
	u8 index;
	if (rem.x() > 0.5) {
	  if (rem.y() > 0.5) {
	    index = 0;
	  } else {
	    index = 1;
	  }
	} else {
	  if (rem.y() > 0.5) {
	    index = 2;
	  } else {
	    index = 3; 
	  }
	}
      
	BiomeCenter* c = closest_biome_centers[index];
	TreeType type;

	Vec2f offset = Vec2f(0,0) -tree_grid.step*0.5 + Vec2f
	    ((global_random()/(f32)global_random.max())
	     *tree_grid.step.x(),
	     (global_random()/(f32)global_random.max())
	     *tree_grid.step.y());
	
	Vec2f p = point + offset;
	Vec2f new_pos_grid = rel_pos_grid + offset;
	
	if (new_pos_grid.x() < tree_grid.size.x() &&
	    new_pos_grid.x() > 0 &&
	    new_pos_grid.y() < tree_grid.size.y() &&
	    new_pos_grid.y() > 0 &&
	    c->treeAtPoint(p, &type)) {
	  chunk_trees.emplace_back(p, type);
	}
      }
      
      biome_data->levels = biLerp(rem, biome_array);
      /*if (ret > SNOW_BOTTOM_LEVEL) {
	f32 snow_level = Noise::fractal
	  ([this](Vec2f in, i32 index) -> f32 {
	    return noise[index].getValue(in);
	  }, point/50.0, 3)*50.0 + interpFunc(interpFunc((ret-SNOW_BOTTOM_LEVEL)/SNOW_LEVEL_DIFF))*255.0;
	if (snow_level > 255) snow_level = 255;
	if (snow_level < 0) snow_level = 0;
	biome_data->snow_level = snow_level;
	biome_data->levels.xyz() *= (255 - biome_data->snow_level);
	Log::message(to_string(biome_data->levels) + to_string(ret));
	}*/
    }
    return ret;
  }

  Array<TreeData> TerrainGenerator::getChunkTrees() {
    return chunk_trees;
  }
}
