#include "Terrain.hpp"
#include "TerrainGenerate.hpp"

NAMESPACE {

  const f32 TERRAIN_CHUNK_SIZE = 10.0f;
  //width of the chunk in vertices
  const u32 TERRAIN_CHUNK_RES = 20;
  const f32 TERRAIN_CHUNK_STEP
    = TERRAIN_CHUNK_SIZE / (f32) TERRAIN_CHUNK_RES;

  Texture* Terrain::TEXTURE = NULL;
  Array<u32> TerrainChunk::mesh_elems;

  CONSTRUCT_ASSET(TERRAIN_TEXTURE_LOADER);
  
  Terrain::TEXTURE = new Texture();
  Terrain::TEXTURE->use();
  Terrain::TEXTURE->load("Terrain", Shader::UNI_TEXTURE);

  for (u8 x = 1; x < TERRAIN_CHUNK_SIZE; ++x) {
    for (u8 y = 1; y < TERRAIN_CHUNK_SIZE; ++y) {
      
      TerrainChunk::mesh_elems.push_back(x*TERRAIN_CHUNK_SIZE+y-1);
      TerrainChunk::mesh_elems.push_back(x*TERRAIN_CHUNK_SIZE+y);
      TerrainChunk::mesh_elems.push_back((x-1)*TERRAIN_CHUNK_SIZE+y);
      
      TerrainChunk::mesh_elems.push_back(x*TERRAIN_CHUNK_SIZE+y-1);
      TerrainChunk::mesh_elems.push_back((x-1)*TERRAIN_CHUNK_SIZE+y);
      TerrainChunk::mesh_elems.push_back((x-1)*TERRAIN_CHUNK_SIZE+y-1);
      
    }
  }
  
  DELETE_ASSET;
  END_ASSET;

  TerrainChunk::TerrainChunk(Vec3f pos, Engine* engine)
    : GameObject(NULL,
		 Material::STATIC,
		 pos,
		 Vec3f(0,0,0),
		 PHYSICS_STATIC),
    mesh({},mesh_elems,Terrain::TEXTURE) {
    this->addRenderable(&mesh);
  }

  BoundingObject TerrainChunk::getBoundingObject() {
    BoundingObject ret(BOUNDING_GROUND);
    ret.ground.dataAtPoint = [] (Vec2f pos, Vec3f* norm)->f32 {
      *norm = Vec3f(0,0,1);
      return 1;
    };
    return ret;
  }
  
  BoundingObject TerrainChunk::getPhysicalBoundingObject() {
    return getBoundingObject();
  }
  
  BoundingObject TerrainChunk::getPrimaryBoundingObject() {
    return getBoundingObject();
  }

  Terrain::Terrain(Engine* _engine) : engine(_engine) {}

  void Terrain::generate(Vec2<u32> size) {
    debugAssert(chunks.size() == 0,
		"Terrain chunk array should be empty before generation");
    for (u32 chunk_x = 0; chunk_x < size.x(); ++chunk_x) {
      for (u32 chunk_y = 0; chunk_y < size.y(); ++chunk_y) {

	Vec3f pos = Vec3f(chunk_x*TERRAIN_CHUNK_SIZE,
			  chunk_y*TERRAIN_CHUNK_SIZE,
			  0);
	TerrainChunk c(pos, engine);

	for (f32 x=0; x<=TERRAIN_CHUNK_SIZE; x+= TERRAIN_CHUNK_STEP) {
	  for (f32 y=0; y<=TERRAIN_CHUNK_SIZE; y+= TERRAIN_CHUNK_STEP) {
	    c.mesh.data.push_back(TerrainGenerate::dataAtPos
			     (Vec2f(x+pos.x(),y+pos.y())));
	  }
	}

	c.mesh.init();
	chunks.push_back(c);
	
      }
    }
  }

}
