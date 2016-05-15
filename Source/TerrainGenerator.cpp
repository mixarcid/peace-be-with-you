#include "TerrainGenerator.hpp"
#include "Terrain.hpp"
#include "Interp.hpp"
#include "Time.hpp"

NAMESPACE {

  Random TerrainGenerator::global_random;
  const u8 TerrainGenerator::NUM_BIOME_TYPES = 5;
  const f64 TerrainGenerator::SEA_FLOOR_HEIGHT = -10;
  const f64 TerrainGenerator::SEA_LEVEL = 0;
  const f64 TerrainGenerator::SNOW_TOP_LEVEL = 600;
  const f64 TerrainGenerator::SNOW_BOTTOM_LEVEL = 450;
  const f64 TerrainGenerator::SNOW_LEVEL_DIFF = SNOW_TOP_LEVEL - SNOW_BOTTOM_LEVEL;
  const f64 TerrainGenerator::MOUNTAIN_HEIGHT_MIN = 300;
  const f64 TerrainGenerator::MOUNTAIN_HEIGHT_DIFF = 400;
  
  inline f64 interpFunc(f64 t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
  }

  inline Vec2d interpFunc(Vec2d val) {
    return Vec2d(interpFunc(val.x()),
		 interpFunc(val.y()));
  }

  inline f64 grassFunc(BiomeCenter* c, Vec2d pos) {
    //return c->noise.getValue(pos/100);
    return Noise::fractal
      ([c](Vec2d in, i32 index) -> f64 {
	return 0.5 + 0.5*c->noise.getValue(in, index);
      }, pos/250.0, 2);
  }

  inline f64 rockFunc(BiomeDetailData* d, Vec2d pos) {
    static FrankNoise noise;
    if (d->rockiness > 0.0) {
      f64 tmp = d->rockiness + (1-d->rockiness)*(noise.getValue(pos*d->rock_small_freq, 0)*0.5 + 0.5);
      return interpFunc(interpFunc(interpFunc(noise.getValue(pos*d->rock_large_freq, 1)*0.5 + 0.5)*tmp));
    } else {
      return 0.0;
    }
  };

  Vec2d PERLIN_OFFSET = Vec2d(1000, 2000);

  f64 BiomeDetailData::detailAtPoint(Vec2d pos,
				     BiomeData* biome_data) {
    static FrankNoise noise;
    f64 rockval = rockFunc(this, pos);
    if (biome_data) {
      u16 rock_level = biome_data->rock_level + (rockval*255.0);
      biome_data->rock_level = (rock_level > 255) ? 255 : rock_level;
      biome_data->grass_level *= (1-rockval);
      biome_data->snow_level *= (1-rockval);
      biome_data->sand_level *= (1-rockval);
    }

    f64 rock = Noise::fractal
      ([this](Vec2d in, i32 index) -> f64 {
	f64 val = noise.getValue(in, index);
	if (abs(val) < 0.1) {
	  val = val*val/0.1;
	} else {
	  val = abs(val);
	}
	return 1.0 - val;
      }, pos/100, 3, 2.0, 0.5);
    return rockval*rock*rock_amplitude;
  }
    
  f64 BiomeCenter::dataAtPoint(Vec2d old_pos,
			       Vec2d pos,
			       BiomeData* biome_data) {

    f64 ret = height;
    rand.seed(rand_seed);
    
    switch(biome) {

    case BIOME_TEST:
      if (biome_data) {
	biome_data->grass_level = 255;
      }
      break;
      
    case BIOME_OCEAN:

      if (biome_data) {
	biome_data->sand_level = 255;
      }
      ret += 5*(0.2 - noise.getValue(old_pos/30)*0.5);
      break;

    case BIOME_GRASSLAND:

      if (biome_data) {
	biome_data->grass_level = 255;
      }
      ret += height + 100.0*amplitude*grassFunc(this, old_pos*freq);
      break;

    case BIOME_JUNGLE:
      {

	f64 h = Noise::fractal
	  ([this](Vec2d in, i32 index) -> f64 {
	    return 0.4 + 0.6*noise.getValue(in, index);
	  }, old_pos*freq/400.0, 3, 2.0, 0.5)*200*amplitude;

	ret += h;

	if (biome_data) biome_data->grass_level = 255;
      
      }
      break;
      
    case BIOME_MOUNTAIN:
      {
	f64 val = Noise::fractal
	  ([this](Vec2d in, i32 index) -> f64 {
	    f64 val = noise.getValue(in, index);
	    if (abs(val) < 0.1) {
	      val = val*val/0.1;
	    } else {
	      val = abs(val);
	    }
	    return 0.5 - val;
	    }, old_pos*freq/700.0, 7, 2.0, 0.5);

	Vec2d offset((rand()/(f64)rand.max()*gen->step.x()*0.5),
		     (rand()/(f64)rand.max()*gen->step.x()*0.5));
	Vec2d center_dist = (pos - center);
	center_dist.x() = 0.5*abs(center_dist.x()/(gen->step.x()))*1.414;
	center_dist.y() = 0.5*abs(center_dist.y()/(gen->step.y()))*1.414;
	//if (center_dist.x() < 0) center_dist.x() = 0;
	//if (center_dist.y() < 0) center_dist.y() = 0;

	f64 mountain_height = (TerrainGenerator::MOUNTAIN_HEIGHT_MIN + TerrainGenerator::MOUNTAIN_HEIGHT_DIFF*(2.0*amplitude - 1.0));
        ret += 150*val + ((1.0/(1.0 + (center_dist.norm())))*2*mountain_height - mountain_height);
	//Log::message(to_string(ret));
	if (biome_data) {
	  biome_data->rock_level = 255;
	}
      }
      break;
      
    case BIOME_DESERT:
      {
	f64 arr[4] = {dir.x(), dir.y(), -dir.y(), dir.x()};
	Mat2d rot(arr);
	Vec2d rot_pos(rot*old_pos);
	Vec2d off_pos(rot_pos.x()*0.3, rot_pos.y()*0.7);
	//Log::message(to_string(dir));
	f64 mag = 1.0;//0.5 + 0.5*noise.getValue(old_pos/100.0, 1);
	ret += height + 200.0*amplitude*((mag)*noise.getValue(off_pos*freq/200.0, 2)*0.5 + 0.5);
	if (biome_data) {
	  biome_data->sand_level = 255;
	}
      }
      break;

    case BIOME_SERENGETI:
      {

	f64 grass_large = (0.5 + 0.5*noise.getValue(old_pos/250.0, 0));
	f64 grass_small = interpFunc(interpFunc(0.5 + 0.5*noise.getValue(old_pos*freq/100.0, 2)*grass_large));
	f64 h = 20*amplitude*(0.5 + 0.5*noise.getValue(old_pos/200.0, 3));
	f64 grass_val = (0.5 + 0.5*grass_small*noise.getValue(old_pos/100.0, 4))*30*amplitude;
	ret += h + grass_val;

	if (biome_data) {
	  biome_data->grass_level = (grass_small*255.0);
	  biome_data->sand_level = 255 - biome_data->grass_level;
	}

      }
      break;
      
    }
    return ret;
  }

  bool BiomeCenter::treeAtPoint(Vec2d pos, TreeType* type) {

    bool ret = false;
    switch(biome) {

    case BIOME_GRASSLAND:
      {
        /*ret = true;
	f64 val =  Noise::fractal
	  ([this](Vec2d in, i32 index) -> f64 {
	    return noise.getValue(in);
	  }, pos/100 + Vec2d(900, 600), 5);
	if (val > 0.7) {
	  *type = TREE_WILLOW;
	} else if (val > 0.5) {
	  *type = TREE_PINE;
	} else if (val > 0.3) {
	  *type = TREE_ELM;
	} else {
	  ret = false;
	  }*/
	break;
      }

    case BIOME_JUNGLE:
      {
	ret = true;
	f64 val =  Noise::fractal
	  ([this](Vec2d in, i32 index) -> f64 {
	    return noise.getValue(in);
	  }, pos/100 + Vec2d(900, 600), 5);
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

    case BIOME_SERENGETI:
      {
	f64 val = (TerrainGenerator::global_random()/
		   (f64)TerrainGenerator::global_random.max());
	if (val > 0.9) {
	  ret = true;
	  *type = TREE_BAOBAB;
	}
      }
      

    default:
      break;
      
    }

    if (ret &&
	rockFunc(&detail, pos) > 0.1) {
      return false;
    }
    
    return ret;
  }
    
  TerrainGenerator::TerrainGenerator(Terrain* _ter,
				     Vec3d _pos,
				     Vec2d _size,
				     Vec2u _num_biomes,
				     Vec2d _chunk_size,
				     Vec2d _internal_step)
    : ter(_ter),
    pos(_pos),
    size(_size),
    num_biomes(_num_biomes),
    step(size.x()/num_biomes.x(),
	 size.y()/num_biomes.y()),
    internal_step(_internal_step),
    pos_offset(pos.xy() -
	       (Vec2d((num_biomes.x()-1)*step.x(),
		      (num_biomes.y()-1)*step.y())/2)),
    tree_grid(Vec2u(_chunk_size/30), _chunk_size) {
      
    //maximum radius for the island
    f64 max_radius = (size.x()/2)*0.9;
    Time t;
    t.makeCurrent();
    u32 seed = t.getMilliseconds();
    global_random.seed(seed);

    u32 size = (num_biomes.x()+4)*(num_biomes.y()+4);
    biome_centers.reserve(size);
      
    //create grid specifying biome centers
    for (i32 x = -2; x < (i32)num_biomes.x() + 2; ++x) {
      for (i32 y = -2; y < (i32)num_biomes.y() + 2; ++y) {

	f64 f = global_random()/(f64)global_random.max();
	Vec2d dir(f, sqrt(1.0 - f*f));
	f64 amplitude = 0.5 + (global_random()/(f64)global_random.max())*0.5;
	f64 freq = 0.5 + (global_random()/(f64)global_random.max())*0.5;

        Vec2d center = pos_offset +
	  Vec2d(step.x()*x, step.y()*y);
	f64 center_dist = (center - pos.xy()).norm()/max_radius;
	
        f64 height = 0;
	BiomeType biome = BIOME_SERENGETI;
	
	/*if (center_dist > 1.0) {
	  biome = BIOME_OCEAN;
	  height = SEA_FLOOR_HEIGHT;
	} else {
	  height = ((noise[0].getValue(center/5000.0)*0.5 + 0.5)*0.75 + 0.25*(global_random()/(f64)global_random.max()))*(1.0 - center_dist)*100.0;
	  f64 mountain_val = ((noise[1].getValue(center/5000.0)*0.5 + 0.5)*0.5*(1.0 - center_dist) + 0.5*(global_random()/(f64)global_random.max()));
	  if (mountain_val > 0.5) {
	    biome = BIOME_MOUNTAIN;
	  } else {
	    f64 biome_val = (noise[2].getValue(center/5000.0)*0.5 + 0.5)*0.5 + 0.5*(global_random()/(f64)global_random.max());
	    if (biome_val > 0.85) {
	      biome = BIOME_SERENGETI;
	    } else if (biome_val > 0.66) {
	      biome = BIOME_GRASSLAND;
	    } else if (biome_val > 0.33) {
	      biome = BIOME_DESERT;
	    } else {
	      biome = BIOME_JUNGLE;
	    }
	    }
	    }*/
	
	BiomeDetailData& d = biome_centers.emplace_back
	  (this, center, dir, height, amplitude, freq, ++seed, biome)->detail;
	
	d.rockiness = (global_random()/(f64)global_random.max())*6.0 - 5.4;
	if (d.rockiness < 0) d.rockiness = 0.0;
	d.rock_amplitude = 10.0 + (global_random()/(f64)global_random.max())*15.0;
	d.rock_large_freq = 0.015 + (global_random()/(f64)global_random.max())*0.01;
	d.rock_small_freq = 0.0015 + (global_random()/(f64)global_random.max())*0.001;
	  
      }
    }
    
  }

  void TerrainGenerator::setChunkCenter(Vec2d chunk_center) {
    prev_grid_pos = Vec2i(-1,-1);
    tree_grid.center = chunk_center;
    chunk_trees.clear();
  }
  
  f64 TerrainGenerator::dataAtPoint(Vec2d point,
				    BiomeData* biome_data) {

    Vec2d input = Vec2d(point.x()/(2*step.x()),
			point.y()/(2*step.y()));
    Vec2d point_offset
    (noise[0].getValue(input)*step.x()*0.6,
      noise[1].getValue(input)*step.y()*0.6);
    Vec2d point_offset_small
      (noise[2].getValue(input*0.5)*step.x()*0.3,
       noise[3].getValue(input*0.5)*step.y()*0.3);
    Vec2d new_point = point + point_offset + point_offset_small;
    Vec2d rel_pos = new_point - pos_offset + size/2;
    //I honestly have no idea
    rel_pos -= Vec2d(step.x()*(num_biomes.x()/2.0-2),
		     step.y()*(num_biomes.y()/2.0-2));
    
    Vec2u indexes(floor(rel_pos.x()/step.x()),
		  floor(rel_pos.y()/step.y()));
    Vec2d rel(rel_pos.x() - indexes.x()*step.x(),
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

    Vec2d rem = new_point - closest_biome_centers[0]->center;
    rem.x() /= step.x();
    rem.y() /= step.y();
    rem = Vec2d(1.0,1.0) - rem;

    f64 heights[4];
    Vec4ub biome_array[4];

    rem = interpFunc(interpFunc(rem));

    rem.x() = rem.x() < 0.0 ? 0.0 : rem.x();
    rem.x() = rem.x() > 1.0 ? 1.0 : rem.x();
    rem.y() = rem.y() < 0.0 ? 0.0 : rem.y();
    rem.y() = rem.y() > 1.0 ? 1.0 : rem.y();

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
    f64 ret;

    for (u8 n=0; n<4; ++n) {
      BiomeData b;
      heights[n] = closest_biome_centers[n]
	->dataAtPoint(point, point, biome_data ? &b : NULL) +
	closest_biome_centers[n]
	->detail.detailAtPoint(point, biome_data ? &b : NULL);
      biome_array[n] = b.levels;
    }
      
    if (biome_data) {
      biome_data->levels = biLerp(rem, biome_array);
    }

    ret = biLerp(rem, heights) ;
    
    if (biome_data) {
      
      Vec2d rel_pos_grid = point - tree_grid.center + tree_grid.size/2;
      Vec2i grid_pos(rel_pos_grid.x()/tree_grid.step.x(),
		     rel_pos_grid.y()/tree_grid.step.y());
      //Log::message(to_string(grid_pos.x()) + to_string(rel_pos_grid) + to_string(internal_step));
      if ((grid_pos.x() > prev_grid_pos.x() ||
	   grid_pos.y() > prev_grid_pos.y()) &&
	  grid_pos.x() < (i32) tree_grid.res.x() -1 &&
	  grid_pos.y() < (i32) tree_grid.res.y() -1) {

	prev_grid_pos = grid_pos;
	
	TreeType type;

	Vec2d offset = Vec2d(0,0) -tree_grid.step*0.5 + Vec2d
	    ((global_random()/(f64)global_random.max())
	     *tree_grid.step.x(),
	     (global_random()/(f64)global_random.max())
	     *tree_grid.step.y());
	
	Vec2d p = point + offset;
	Vec2d new_pos_grid = rel_pos_grid + offset;
	
	if (new_pos_grid.x() < tree_grid.size.x() &&
	    new_pos_grid.x() > 0 &&
	    new_pos_grid.y() < tree_grid.size.y() &&
	    new_pos_grid.y() > 0 &&
	    c->treeAtPoint(p, &type)) {
	  chunk_trees.emplace_back(p, type);
	}
      }
      
      if (ret > SNOW_BOTTOM_LEVEL) {
	f64 snow_level = Noise::fractal
	  ([this](Vec2d in, i32 index) -> f64 {
	    return noise[index].getValue(in);
	  }, point/50.0, 3)*50.0 + interpFunc(interpFunc((ret-SNOW_BOTTOM_LEVEL)/SNOW_LEVEL_DIFF))*255.0;
	if (snow_level > 255) snow_level = 255;
	if (snow_level < 0) snow_level = 0;
	biome_data->snow_level = snow_level;
	biome_data->levels.xyz() *= (255 - biome_data->snow_level);
	//Log::message(to_string(biome_data->levels) + to_string(ret));
      }
    }
    return ret;
  }

  Array<TreeData> TerrainGenerator::getChunkTrees() {
    return chunk_trees;
  }
}
