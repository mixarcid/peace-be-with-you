#include "Terrain.hpp"
#include "TerrainGenerator.hpp"

NAMESPACE {

  const f32 TERRAIN_CHUNK_SIZE = 10.0f;
  //width of the chunk in vertices
  const u32 TERRAIN_CHUNK_RES = 20;
  const f32 TERRAIN_CHUNK_STEP
    = TERRAIN_CHUNK_SIZE / (f32) TERRAIN_CHUNK_RES;

  Asset<Texture> Terrain::texture("Terrain");
  Array<StaticMesh> Terrain::chunk_meshes;
  Array<u32> Terrain::mesh_elems;
  EBO Terrain::elem_buffer;

  f32 test(Vec2f pos, Vec3f* norm) {
    *norm = Vec3f(0,0,1);
    return 1;
  }

  void TerrainChunk::init() {
    addComponent(Terrain::chunk_meshes.push_back(*Terrain::texture.get()));
    tight_object.ground = BoundingGround(test);
    loose_object.object = BoundingObject();
  }

  Terrain::Terrain(Engine* _engine) : engine(_engine) {

    for (u8 x = 1; x < TERRAIN_CHUNK_RES; ++x) {
      for (u8 y = 1; y < TERRAIN_CHUNK_RES; ++y) {
	
	mesh_elems.push_back(x*TERRAIN_CHUNK_RES+y-1);
	mesh_elems.push_back(x*TERRAIN_CHUNK_RES+y);
	mesh_elems.push_back((x-1)*TERRAIN_CHUNK_RES+y);
	
	mesh_elems.push_back(x*TERRAIN_CHUNK_RES+y-1);
	mesh_elems.push_back((x-1)*TERRAIN_CHUNK_RES+y);
	mesh_elems.push_back((x-1)*TERRAIN_CHUNK_RES+y-1);
	
      }
    }
    Shader::setFlags(SHADER_NO_FLAGS);
    elem_buffer.init();
    elem_buffer.bindArray(mesh_elems,false);
    
  }

  void Terrain::generate(Vec2u size) {
    
    for (u32 chunk_x = 0; chunk_x < size.x(); ++chunk_x) {
      for (u32 chunk_y = 0; chunk_y < size.y(); ++chunk_y) {

	Vec3f pos = Vec3f(chunk_x*TERRAIN_CHUNK_SIZE/2,
			  chunk_y*TERRAIN_CHUNK_SIZE/2,
			  0);
	Pointer<TerrainChunk> c = engine->emplaceObject<TerrainChunk>(pos);
	Pointer<StaticMesh> mesh = (Pointer<StaticMesh>) c->getComponent<RenderableComp>();

        for (u8 x = 0; x < TERRAIN_CHUNK_RES; ++x) {
	  for (u8 y = 0; y < TERRAIN_CHUNK_RES; ++y) {
	    mesh->data.push_back(TerrainGenerator::dataAtPos
			     (Vec2f(TERRAIN_CHUNK_STEP*x+pos.x(),TERRAIN_CHUNK_STEP*y+pos.y())));
	  }
	} 

	mesh->init(elem_buffer);
	
      }
    }
  }

}
