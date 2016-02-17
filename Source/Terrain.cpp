#include "Terrain.hpp"
#include "TerrainGenerator.hpp"

NAMESPACE {

  const f32 Terrain::CHUNK_SIZE = 75.0f;
  //width of the chunk in vertices
  //must be power of 2 plus 2 (not entirely sure why)
  const u32 Terrain::CHUNK_RES = 130;//258;
  const f32 Terrain::CHUNK_STEP
    = 2*(Terrain::CHUNK_SIZE / (f32) Terrain::CHUNK_RES);

  const u8 MID_REDUCTION = 2;
  const u8 SMALL_REDUCTION = 4;

  Asset<Texture> Terrain::texture("Terrain");
  Array<TerrainRenderable> Terrain::chunk_meshes;
  EBO Terrain::elem_buffer_large;
  EBO Terrain::elem_buffer_mid;
  EBO Terrain::elem_buffer_small;

  f32 test(Vec2f pos, Vec3f* norm) {
    *norm = Vec3f(0,0,1);
    return 1;
  }

  void TerrainChunk::init() {
    addComponent(Terrain::chunk_meshes.emplace_back());
    tight_object.ground = BoundingGround(test);
    loose_object.object = BoundingObject();
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

  Terrain::Terrain(Engine* _engine) : engine(_engine) {

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

    Shader::setFlags(SHADER_NO_FLAGS);
    elem_buffer_small.init();
    elem_buffer_small.bindArray(mesh_elems_small,false);
    elem_buffer_mid.init();
    elem_buffer_mid.bindArray(mesh_elems_mid,false);
    elem_buffer_large.init();
    elem_buffer_large.bindArray(mesh_elems_large,false);
    
  }

  void Terrain::generate(Vec3f pos, Vec2u size) {

    TerrainGenerator gen;

    Vec3f offset = pos -
      (Vec3f(size.x()-1, size.y()-1, 0)
       *(CHUNK_SIZE-CHUNK_STEP)/2);
    
    for (u32 chunk_x = 0; chunk_x < size.x(); ++chunk_x) {
      for (u32 chunk_y = 0; chunk_y < size.y(); ++chunk_y) {
	
	Vec2f position = offset.xy() + Vec2f
	  (chunk_x*(CHUNK_SIZE-CHUNK_STEP),
	   chunk_y*(CHUNK_SIZE-CHUNK_STEP));
	
	Pointer<TerrainChunk> c = engine->emplaceObject<TerrainChunk>
	  (Vec3f(position.x(), position.y(), offset.z()));
	//Log::message(to_string(position));
	
	Pointer<TerrainRenderable> mesh = Pointer<TerrainRenderable>
	  (c->getComponent<RenderableComp>());
	Array<BasicMeshData> data(sqr(CHUNK_RES));
	
        for (u16 x = 0; x < CHUNK_RES; ++x) {
	  for (u16 y = 0; y < CHUNK_RES; ++y) {

	    Vec2f local_pos(CHUNK_STEP*x - CHUNK_SIZE/2,
			    CHUNK_STEP*y - CHUNK_SIZE/2);
	    f32 height = gen.heightAtPoint(local_pos + position);
	    Vec2f tex_coord = gen.texCoordAtPoint(local_pos + position);
	    data.push_back
	      (BasicMeshData(Vec3f(local_pos.x(),
				   local_pos.y(),
				   height + offset.z()),
			     Vec3f(0,0,1),
			     tex_coord));;
	  }
	}

	//Time to compute some normals!
	for (u16 x = 0; x < CHUNK_RES; ++x) {
	  for (u16 y = 0; y < CHUNK_RES; ++y) {

	    BasicMeshData& d = data[x*CHUNK_RES+y];
	    Vec2f local_pos = d.pos.xy();
	    Vec3f points[4];
	    
	    if (x > 0) {
	      points[0] = data[(x-1)*CHUNK_RES+y].pos;
	    } else {
	      Vec2f p(local_pos.x()-CHUNK_STEP, local_pos.y());
	      points[0] = Vec3f(p.x(), p.y(),
				gen.heightAtPoint
				(position + p));
	    }
	    if (y > 0) {
	      points[1] = data[x*CHUNK_RES+(y-1)].pos;
	    } else {
	      Vec2f p(local_pos.x(), local_pos.y()-CHUNK_STEP);
	      points[1] = Vec3f(p.x(), p.y(),
				gen.heightAtPoint
				(position + p));
	    }
	    if (x < CHUNK_RES-1) {
	      points[2] = data[(x+1)*CHUNK_RES+y].pos;
	    } else {
	      Vec2f p(local_pos.x()+CHUNK_STEP, local_pos.y());
	      points[2] = Vec3f(p.x(), p.y(),
				gen.heightAtPoint
				(position + p));
	    }
	    if (y < CHUNK_RES-1) {
	      points[3] = data[x*CHUNK_RES+(y+1)].pos;
	    } else {
	      Vec2f p(local_pos.x(), local_pos.y()+CHUNK_STEP);
	      points[3] = Vec3f(p.x(), p.y(),
				gen.heightAtPoint
				(position + p));
	    }
	    d.norm = computeNormal(d.pos, points);
	  }
	}

	mesh->init(data);
	
      }
    }
  }

}
