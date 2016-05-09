#include "Terrain.hpp"
#include "TerrainGenerator.hpp"
#include "Assets.hpp"
#include "FileIO.hpp"
#include "VectorIO.hpp"
#include "Interp.hpp"
#include "Water.hpp"

NAMESPACE {

  const f32 Terrain::CHUNK_SIZE = 500.0f;//100.0f;
  //width of the chunk in vertices
  //must be power of 2 minus 1
  const u32 Terrain::CHUNK_RES = 127;
  const f32 Terrain::CHUNK_STEP
    = (Terrain::CHUNK_SIZE / (f32) Terrain::CHUNK_RES);

  const u32 Terrain::CHUNKS_IN_VIEW = 5;
  
  const u8 MID_REDUCTION = 2;
  const u8 SMALL_REDUCTION = 4;

  Asset<Texture> Terrain::texture("Terrain");
  Array<TerrainRenderable> Terrain::chunk_meshes;
  HashMap<Vec2u, Pointer<TerrainChunk>> Terrain::chunks;
  Mutex Terrain::chunk_mutex;
  EBO Terrain::elem_buffer_large;
  EBO Terrain::elem_buffer_mid;
  EBO Terrain::elem_buffer_small;

  Array<Vec2f> Terrain::pos_data;
  VBO Terrain::pos_buffer;

  const StaticMaterial TerrainGround::MATERIAL(0.7, 0.6);
  
  Terrain* global_terrain;
  
  void TerrainChunk::init() {
    addComponent(new TerrainChunkComp());
  }

  void TerrainChunk::removeTrees() {
    auto& comp = getComponent<TerrainChunkComp>();
    if (comp) {
      for (auto& tree : comp->trees) {
	Engine::removeStatic(tree);
      }
    }
  }

  TerrainChunk::~TerrainChunk() {
    auto& rend = getComponent<RenderableComp>();
    if (rend) {
      Terrain::chunk_meshes.removeAndReplace
	((Pointer<TerrainRenderable>&)rend);
    }
    auto& comp = getComponent<TerrainChunkComp>();
    delete comp;
  }

  Vec3f computeNormal(Vec3f point,
		      Vec3f adjacent_points[4]) {
    Vec3f normal;
    for (u8 n=0; n<4; ++n) {
      normal += Vec3f::cross
	(point-adjacent_points[n],
	 point-adjacent_points[(n+1)%4]);
    }
    return normal.normalized();
  }

  f32 ground_func(Vec2f p, Vec3f* norm) {
    return global_terrain->heightAtPoint(p, norm);
  }

  void TerrainGround::init(Terrain* ter) {
    Pointer<TerrainGround> t(this);
    addComponent(new StaticPhysicsComp
		 ((Pointer<StaticObject>&)t, MATERIAL));

    global_terrain = ter;
    BoundingGround bound(ground_func);
    tight_object.set(&bound);

    BoundingObject all(BoundingObject::ALL);
    loose_object.set(&all);
  }

  TerrainGround::~TerrainGround() {
    delete getComponent<StaticPhysicsComp>();
  }

  void TerrainGround::message(Message* msg) {
    switch(typeId(msg)) {
    case typeId<OutOfBoundsMessage>():
      ((OutOfBoundsMessage*)msg)->should_insert = true;
      break;
    }
  }

  Terrain::Terrain()
    : file(NULL) {

    Array<u32> mesh_elems_small;
    Array<u32> mesh_elems_mid;
    Array<u32> mesh_elems_large;

    for (u16 x = 1; x < CHUNK_RES; ++x) {
      for (u16 y = 1; y < CHUNK_RES; ++y) {
	
	mesh_elems_large.push_back(x*CHUNK_RES+y-1);
	mesh_elems_large.push_back(x*CHUNK_RES+y);
	mesh_elems_large.push_back((x-1)*CHUNK_RES+y);
	
	mesh_elems_large.push_back(x*CHUNK_RES+y-1);
	mesh_elems_large.push_back((x-1)*CHUNK_RES+y);
	mesh_elems_large.push_back((x-1)*CHUNK_RES+y-1);
	
	if (x >= MID_REDUCTION &&
	    y >= MID_REDUCTION &&
	    x % MID_REDUCTION == 0 &&
	    y % MID_REDUCTION == 0) {

	  mesh_elems_mid.push_back(x*CHUNK_RES+y-MID_REDUCTION);
	  mesh_elems_mid.push_back(x*CHUNK_RES+y);
	  mesh_elems_mid.push_back((x-MID_REDUCTION)*CHUNK_RES+y);
	
	  mesh_elems_mid.push_back(x*CHUNK_RES+y-MID_REDUCTION);
	  mesh_elems_mid.push_back((x-MID_REDUCTION)*CHUNK_RES+y);
	  mesh_elems_mid.push_back((x-MID_REDUCTION)*CHUNK_RES+y-MID_REDUCTION);
	  
	}

	if (x >= SMALL_REDUCTION &&
	    y >= SMALL_REDUCTION &&
	    x % SMALL_REDUCTION == 0 &&
	    y % SMALL_REDUCTION == 0) {

	  mesh_elems_small.push_back(x*CHUNK_RES+y-SMALL_REDUCTION);
	  mesh_elems_small.push_back(x*CHUNK_RES+y);
	  mesh_elems_small.push_back((x-SMALL_REDUCTION)*CHUNK_RES+y);
	
	  mesh_elems_small.push_back(x*CHUNK_RES+y-SMALL_REDUCTION);
	  mesh_elems_small.push_back((x-SMALL_REDUCTION)*CHUNK_RES+y);
	  mesh_elems_small.push_back((x-SMALL_REDUCTION)*CHUNK_RES+y-SMALL_REDUCTION);
	  
	}
	
      }
    }

    for (u16 x = 0; x < CHUNK_RES; ++x) {
      for (u16 y = 0; y < CHUNK_RES; ++y) {
	pos_data.emplace_back(CHUNK_STEP*x - CHUNK_SIZE/2,
			      CHUNK_STEP*y - CHUNK_SIZE/2);
      }
    }
    
    Shader::setFlags(SHADER_NO_FLAGS);
    elem_buffer_small.init();
    elem_buffer_small.bindArray(mesh_elems_small, false);
    elem_buffer_mid.init();
    elem_buffer_mid.bindArray(mesh_elems_mid, false);
    elem_buffer_large.init();
    elem_buffer_large.bindArray(mesh_elems_large, false);
    pos_buffer.init();
    pos_buffer.bindArray(pos_data, false);
    
  }

  Terrain::~Terrain() {
    if (file) fclose(file);
  }

  const static char TER_VERSION = 0x01;
  const static char TER_SIG[3] = {'T', 'E', 'R'};

  void Terrain::generate(String filename,
			 Vec3f pos,
			 Vec2u _size) {

    size = _size;
    chunk_pos_offset = pos -
      (Vec3f(size.x()-1, size.y()-1, 0)
       *(CHUNK_SIZE-CHUNK_STEP)/2);
    if (file) fclose(file); file = NULL;

    String full_name = (DIR_TERRAIN +
			filename +
			DIR_TERRAIN_EXTENSION);
    file = fopen(full_name.c_str(), "wb");
    fatalAssert(file != NULL,
		"Unable to create terrain file %s", full_name.c_str());

    fwrite(&TER_SIG, sizeof(char), 3, file);
    fwrite(&TER_VERSION, sizeof(char), 1, file);

    fio::writeLittleEndian(file, CHUNK_RES);
    save(file, pos);
    save(file, size);

    chunk_table_pos = ftell(file);
    u32 num_chunks = size.x()*size.y();
    for (u32 n=0; n<num_chunks; ++n) {
      u64 val = 0;
      fio::writeLittleEndian<u64>(file, val);
    }

    TerrainGenerator gen(this,
			 pos,
			 size*CHUNK_SIZE,
			 size/4,
			 Vec2f(CHUNK_SIZE,
			       CHUNK_SIZE),
			 Vec2f(CHUNK_STEP,
			       CHUNK_STEP));
    u32 n = 0;
    Array<Vec3f> data;
    data.reserve(sqr(CHUNK_RES));
    
    for (u32 chunk_x = 0; chunk_x < size.x(); ++chunk_x) {
      for (u32 chunk_y = 0; chunk_y < size.y(); ++chunk_y) {
	data.clear();
	u64 chunk_pos = ftell(file);
	
	Vec2f position = chunk_pos_offset.xy() + Vec2f
	  (chunk_x*(CHUNK_SIZE-CHUNK_STEP),
	   chunk_y*(CHUNK_SIZE-CHUNK_STEP));
	save(file, position);
	gen.setChunkCenter(position);

	f32 max_z = -FLT_MAX;
	f32 min_z = FLT_MAX;
	
        for (u16 x = 0; x < CHUNK_RES; ++x) {
	  for (u16 y = 0; y < CHUNK_RES; ++y) {

	    Vec2f local_pos(CHUNK_STEP*x - CHUNK_SIZE/2,
			    CHUNK_STEP*y - CHUNK_SIZE/2);
	    f32 height;
	    BiomeData b;
	    height = gen.dataAtPoint(position + local_pos, &b);
	    max_z = height > max_z ? height : max_z;
	    min_z = height < min_z ? height : min_z;
	    data.push_back(Vec3f(local_pos.x(),
				 local_pos.y(),
				 height));
	    fio::writeLittleEndian(file, height);
	    save(file, b.levels); 
	    
	  }
	}

	//Time to compute some normals!
	Array<Vec3f> norms;
	norms.reserve(CHUNK_RES*CHUNK_RES);
	for (u16 x = 0; x < CHUNK_RES; ++x) {
	  for (u16 y = 0; y < CHUNK_RES; ++y) {

	    Vec3f pos = data[x*CHUNK_RES+y];
	    Vec2f local_pos = pos.xy();
	    Vec3f points[4];
	    
	    if (x > 0) {
	      points[0] = data[(x-1)*CHUNK_RES+y];
	    } else {
	      Vec2f p(local_pos.x()-CHUNK_STEP, local_pos.y());
	      points[0] = Vec3f(p.x(), p.y(),
				gen.dataAtPoint
				(position + p));
	    }
	    if (y > 0) {
	      points[1] = data[x*CHUNK_RES+(y-1)];
	    } else {
	      Vec2f p(local_pos.x(), local_pos.y()-CHUNK_STEP);
	      points[1] = Vec3f(p.x(), p.y(),
				gen.dataAtPoint
				(position + p));
	    }
	    if (x < CHUNK_RES-1) {
	      points[2] = data[(x+1)*CHUNK_RES+y];
	    } else {
	      Vec2f p(local_pos.x()+CHUNK_STEP, local_pos.y());
	      points[2] = Vec3f(p.x(), p.y(),
				gen.dataAtPoint
				(position + p));
	    }
	    if (y < CHUNK_RES-1) {
	      points[3] = data[x*CHUNK_RES+(y+1)];
	    } else {
	      Vec2f p(local_pos.x(), local_pos.y()+CHUNK_STEP);
	      points[3] = Vec3f(p.x(), p.y(),
				gen.dataAtPoint
				(position + p));
	    }
	    Vec3f norm = computeNormal(pos, points);
	    norms.push_back(norm);
	    save(file, norm);
	  }
	}

	//aabb center and halves
	Vec3f center = Vec3f(position.x()-0.5*CHUNK_STEP,
			     position.y()-0.5*CHUNK_STEP,
			     (max_z + min_z)/2);
	save(file, center);
	Vec3f halves = Vec3f(CHUNK_SIZE-CHUNK_STEP,
			     CHUNK_SIZE-CHUNK_STEP,
			     max_z - min_z)/2;
	save(file, halves);

	Array<TreeData> trees = gen.getChunkTrees();
	Array<TreeData> new_trees(trees.size());
	for (u32 n=0; n<trees.size(); ++n) {

	  TreeData* tree = &trees[n];
	  Vec2f local_pos = tree->pos -
	    center.xy() +
	    Vec2f(Terrain::CHUNK_SIZE,
		  Terrain::CHUNK_SIZE)/2;
	  if (local_pos.x() > 0 && local_pos.y() > 0) {
	    Vec2u indexes = local_pos/CHUNK_STEP;
	    Vec3f norm = norms[indexes.x()*CHUNK_RES + indexes.y()];
	    f32 height = data[indexes.x()*CHUNK_RES + indexes.y()].z();
	  
	    if (height > TerrainGenerator::SEA_LEVEL && norm.z() > 0.8) {
	      new_trees.push_back(trees[n]);
	    }
	  }
	}
	
	fio::writeLittleEndian(file, new_trees.size());
	for (TreeData tree : new_trees) {
	  save(file, tree.pos);
	  fio::writeLittleEndian(file, tree.type);
	}
	
	u64 cur_pos = ftell(file);
	fseek(file, chunk_table_pos + (n*sizeof(u64)), SEEK_SET);
	fio::writeLittleEndian(file, chunk_pos);
	fseek(file, cur_pos, SEEK_SET);
	++n;
      }
    }

#ifdef PEACE_LOG_LOADED_ASSETS
    Log::message("Generated terrain %s",
		 filename.c_str());
#endif
    
  }

  void global_scene_update() {
    global_terrain->_scene_update(false);
  }

  void Terrain::loadFile(String filename) {

    if (file) fclose(file); file = NULL;

    String full_name = (DIR_TERRAIN +
			filename +
			DIR_TERRAIN_EXTENSION);
    file = fopen(full_name.c_str(), "rb");
    fatalAssert(file != NULL,
		"Unable to load terrain file %s", full_name.c_str());

    char sig[3];
    char ver;
    fread(sig, sizeof(char), 3, file);
    fread(&ver, sizeof(char), 1, file);
    fatalAssert(sig[0] == TER_SIG[0] &&
		sig[1] == TER_SIG[1] &&
		sig[2] == TER_SIG[2],
		"Incorrect signature for terrain file %s",
		full_name.c_str());
    fatalAssert(ver == TER_VERSION,
		"Incorrect version for terrain file %s",
		full_name.c_str());
    
    fatalAssert(fio::readLittleEndian<u32>(file) == CHUNK_RES,
		"Incorrect resolution for terrain file %s",
		full_name.c_str());

    Vec3f pos;
    load(file, &pos);
    load(file, &size);
    chunk_pos_offset = pos -
      (Vec3f(size.x()-1, size.y()-1, 0)
       *(CHUNK_SIZE-CHUNK_STEP)/2);
    chunk_table_pos = ftell(file);

    ground_object = Engine::emplaceStatic<TerrainGround>(pos, this);
    Engine::emplaceStatic<Water>
      (Vec3f(pos.xy(), TerrainGenerator::SEA_LEVEL),
       Vec2f(10000, 10000));
    Terrain::chunks.reserve(sqr(2*CHUNKS_IN_VIEW+1));

    _scene_update(true);
    Engine::engine->scene_callbacks.push_back(global_scene_update);
    
#ifdef PEACE_LOG_LOADED_ASSETS
    Log::message("Loaded Terrain %s",
		 filename.c_str());
#endif
  }

  template<bool CurScene>
    void finalize_load() {
    global_terrain->chunk_load_mutex.lock();
    
    Terrain::ChunkLoadData* d = global_terrain->chunk_load_data.begin();
    debugAssert(d != global_terrain->chunk_load_data.end(),
		"You're trying to finalize chunk data that doesn't exist");
    
    Terrain::chunk_mutex.lock();
    Log::message("%s, %p", to_string(d->index).c_str(), Terrain::chunks[d->index].data);
    Terrain::chunks[d->index] = d->chunk;
    Terrain::chunk_mutex.unlock();
    
    d->mesh->init();
    d->chunk->addComponent<TerrainRenderable>(d->mesh);
    auto& comp = d->chunk->getComponent<TerrainChunkComp>();
    comp->trees.reserve(d->tree_data.size());
    for (auto& tree : d->tree_data) {
      Pointer<Tree> p = Engine::emplaceStaticNoRegister<Tree>
	(tree.type, Vec3f
	 (tree.pos, global_terrain->heightAtPoint(tree.pos, NULL)));
      Engine::registerStatic(p, CurScene);
      comp->trees.push_back(p);
    }
    
    global_terrain->chunk_load_data.removeAndReplace(d);
    global_terrain->chunk_load_mutex.unlock();
  }
  
  void Terrain::loadChunk(Vec2u index, bool cur_scene) {

    debugAssert(file,
		"You must generate or load a terrain "
		"file before loading chunks");
    
    fseek(file, chunk_table_pos +
	  (index.x()*size.y() + index.y())*sizeof(u64), SEEK_SET);
    fseek(file, fio::readLittleEndian<u64>(file), SEEK_SET);
    	
    Vec2f position;
    load(file, &position);
	
    Pointer<TerrainChunk> c
      (Engine::emplaceStaticNoRegister<TerrainChunk>
       (Vec3f(position.x(),
	      position.y(),
	      chunk_pos_offset.z())));
    
    //Log::message(to_string(position));

    Pointer<TerrainRenderable> mesh = chunk_meshes.emplace_back();
      /*= (Pointer<TerrainRenderable>&)
	c->getComponent<RenderableComp>();*/
    mesh->height_data.reserve(sqr(CHUNK_RES));

    for (u16 x = 0; x < CHUNK_RES; ++x) {
      for (u16 y = 0; y < CHUNK_RES; ++y) {

	f32 height = fio::readLittleEndian<f32>(file);
	BiomeData b;
	load(file, &b.levels);
	mesh->height_data.push_back(height);
	mesh->biome_data.push_back(b);
      }
    }

    for (u16 x = 0; x < CHUNK_RES; ++x) {
      for (u16 y = 0; y < CHUNK_RES; ++y) {
	Vec3f norm;
	load(file, &norm);
	mesh->normal_data.push_back(norm);
      }
    }

    BoundingAABB aabb;
    load(file, &aabb.center);
    load(file, &aabb.halves);
    c->loose_object.set(&aabb);
    c->tight_object.set(c->getLooseBoundingObject());

    u32 num_trees = fio::readLittleEndian<u32>(file);
    Array<TreeData> tree_data(num_trees);
 
    for (u32 n=0; n<num_trees; ++n) {
      Vec2f pos;
      load(file, &pos);
      tree_data.emplace_back(pos, fio::readLittleEndian<TreeType>(file));
    }
    
    Engine::registerStatic(c, cur_scene);

    ChunkLoadData d;
    d.chunk = c;
    d.mesh = mesh;
    d.tree_data = tree_data;
    d.index = index;
    chunk_load_mutex.lock();
    chunk_load_data.push_back(d);
    chunk_load_mutex.unlock();
    
    if (cur_scene) {
      finalize_load<true>();
    } else {
      Engine::engine->synchronized_mutex.lock();
      Engine::engine->synchronized_callbacks.push_back(finalize_load<false>);
      Engine::engine->synchronized_mutex.unlock();
    }
  }

  Vec2u Terrain::chunkAtPoint(Vec2f p) {
    Vec2f rel_pos = p - chunk_pos_offset.xy() +
      Vec2f(Terrain::CHUNK_SIZE,Terrain::CHUNK_SIZE)/2;
    const f32 rel_div = (Terrain::CHUNK_SIZE-Terrain::CHUNK_STEP);
    return Vec2u(rel_pos.x()/rel_div, rel_pos.y()/rel_div);
  }
  
  f32 Terrain::heightAtPoint(Vec2f p, Vec3f* norm) {
    
    Vec2f rel_pos = p - chunk_pos_offset.xy() +
      Vec2f(Terrain::CHUNK_SIZE,Terrain::CHUNK_SIZE)/2;
    const f32 rel_div = (Terrain::CHUNK_SIZE-Terrain::CHUNK_STEP);
    Vec2u chunk_index =  Vec2u(rel_pos.x()/rel_div, rel_pos.y()/rel_div);

    chunk_mutex.lock();
    auto it = Terrain::chunks.find(chunk_index);
    if (rel_pos.x() < 0 ||
	rel_pos.y() < 0 ||
	/*chunk_index.x() >= size.x() ||
	  chunk_index.y() >= size.y() ||*/
        it == Terrain::chunks.end()) {
      if (norm) *norm = Vec3f(0,0,1);
      chunk_mutex.unlock();
      return TerrainGenerator::SEA_FLOOR_HEIGHT;
    }
    chunk_mutex.unlock();
    
    Pointer<TerrainRenderable>& rend =
		      (Pointer<TerrainRenderable>&)
		      it->second->getComponent<RenderableComp>();
    chunk_mutex.unlock();
    
    f32 rel_x = rel_pos.x() -
      chunk_index.x()*rel_div;
    f32 rel_y = rel_pos.y() -
      chunk_index.y()*rel_div;
    u32 x_mesh_index = rel_x;
    u32 y_mesh_index = rel_y;
    Vec2f rem(/*1 -*/ (rel_x - x_mesh_index),
	      /*1 -*/ (rel_y - y_mesh_index));

    x_mesh_index /= Terrain::CHUNK_STEP;
    y_mesh_index /= Terrain::CHUNK_STEP;
	
    f32 heights[4];
    heights[0] = rend->height_data
      [x_mesh_index*Terrain::CHUNK_RES + y_mesh_index];
    heights[1] = rend->height_data
      [(x_mesh_index + 1)*Terrain::CHUNK_RES + y_mesh_index];
    heights[2] = rend->height_data
      [(x_mesh_index)*Terrain::CHUNK_RES + y_mesh_index + 1];
    heights[3] = rend->height_data
      [(x_mesh_index + 1)*Terrain::CHUNK_RES + y_mesh_index + 1];

    if (norm) {
      Vec3f norms[4];
      norms[0] = rend->normal_data
	[x_mesh_index*Terrain::CHUNK_RES + y_mesh_index];
      norms[1] = rend->normal_data
	[(x_mesh_index + 1)*Terrain::CHUNK_RES + y_mesh_index];
      norms[2] = rend->normal_data
	[(x_mesh_index)*Terrain::CHUNK_RES + y_mesh_index + 1];
      norms[3] = rend->normal_data
	[(x_mesh_index + 1)*Terrain::CHUNK_RES + y_mesh_index + 1];
      *norm = biLerp(rem, norms);
    }
    return biLerp(rem, heights);
  }

  void erase_chunk() {
    global_terrain->erase_chunk_mutex.lock();
    Vec2u* v = global_terrain->erase_chunks.begin();
    debugAssert(v != global_terrain->erase_chunks.end(),
		"You're trying to erase a chunk that doesn't exist");
    Terrain::chunk_mutex.lock();
    Pointer<TerrainChunk>& c = Terrain::chunks[*v];
    c->removeTrees();
    Engine::removeStatic(c);
    Terrain::chunks.erase(*v);
    Terrain::chunk_mutex.unlock();
    global_terrain->erase_chunks.removeAndReplace(v);
    global_terrain->erase_chunk_mutex.unlock();
  }
  
  void Terrain::_scene_update(bool cur_scene) {
    
    Vec2i chunk = chunkAtPoint(Engine::engine->graphics.cam->getTrans().xy());
    bool should_load = true;//cur_scene;

    chunk_mutex.lock();
    auto it = Terrain::chunks.begin();
    chunk_mutex.unlock();
    
    while (true) {

      chunk_mutex.lock();
      if (it == Terrain::chunks.end()) break;
      if (abs(chunk.x() - (i32) it->first.cx())
	  > (i32) CHUNKS_IN_VIEW ||
	  abs(chunk.y() - (i32) it->first.cy())
	  > (i32) CHUNKS_IN_VIEW) {
	erase_chunk_mutex.lock();
        erase_chunks.push_back(it->first);
	erase_chunk_mutex.unlock();
	//should_load = true;
      }
      chunk_mutex.unlock();
      ++it;
    }
    chunk_mutex.unlock();
    
    if (should_load) {

      for (u32 n=0; n<erase_chunks.size(); ++n) {
	
	if (cur_scene) {
	  erase_chunk();
	} else {
	  Engine::engine->synchronized_mutex.lock();
	  Engine::engine->synchronized_callbacks.push_back(erase_chunk);
	  Engine::engine->synchronized_mutex.unlock();
	}
	
      }
      
      i32 x = i32(chunk.x()) - i32(CHUNKS_IN_VIEW) - 1;
      while (++x <= i32(chunk.x() + CHUNKS_IN_VIEW)) {
	i32 y = i32(chunk.y()) - i32(CHUNKS_IN_VIEW) - 1;
        while(++y <= i32(chunk.y() + CHUNKS_IN_VIEW)) {
	  Vec2u vec(x, y);
	  if (x > 0 && y > 0 &&
	      x < (i32) size.x() && y < (i32) size.y()) {
	    chunk_mutex.lock();
	    bool should_cont = (Terrain::chunks.find(vec) != Terrain::chunks.end());
	    chunk_mutex.unlock();
	    if (should_cont) continue;
	    loadChunk(vec, cur_scene);
	  }
	}
      }
    }
    
  }
  
}
