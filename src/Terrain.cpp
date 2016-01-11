#include "Terrain.hpp"
#include "TerrainGenerate.hpp"

NAMESPACE {

  const f32 TERRAIN_CHUNK_SIZE = 10.0f;
  //width of the chunk in vertices
  const u32 TERRAIN_CHUNK_RES = 20;
  const f32 TERRAIN_CHUNK_STEP
    = TERRAIN_CHUNK_SIZE / (f32) TERRAIN_CHUNK_RES;

  Texture* Terrain::TEXTURE = NULL;
  Array<u32> Terrain::MESH_ELEMS;
  EBO Terrain::ELEM_BUFFER;

  CONSTRUCT_ASSET(TERRAIN_TEXTURE_LOADER);
  
  Terrain::TEXTURE = new Texture();
  Terrain::TEXTURE->use();
  Terrain::TEXTURE->load("Terrain", Shader::UNI_TEXTURE);

  for (u8 x = 1; x < TERRAIN_CHUNK_RES; ++x) {
    for (u8 y = 1; y < TERRAIN_CHUNK_RES; ++y) {
      
      Terrain::MESH_ELEMS.push_back(x*TERRAIN_CHUNK_RES+y-1);
      Terrain::MESH_ELEMS.push_back(x*TERRAIN_CHUNK_RES+y);
      Terrain::MESH_ELEMS.push_back((x-1)*TERRAIN_CHUNK_RES+y);
      
      Terrain::MESH_ELEMS.push_back(x*TERRAIN_CHUNK_RES+y-1);
      Terrain::MESH_ELEMS.push_back((x-1)*TERRAIN_CHUNK_RES+y);
      Terrain::MESH_ELEMS.push_back((x-1)*TERRAIN_CHUNK_RES+y-1);
      
    }
  }

  Shader::setFlags(TerrainMesh::FLAGS);
  Terrain::ELEM_BUFFER.init();
  Terrain::ELEM_BUFFER.bindArray(Terrain::MESH_ELEMS,false);
  
  DELETE_ASSET;
  END_ASSET;

  TerrainChunk::TerrainChunk(Vec3f pos)
    : GameObject(NULL,
		 Material::STATIC,
		 pos,
		 Vec3f(0,0,0),
		 PHYSICS_STATIC),
    mesh(&Terrain::ELEM_BUFFER,Terrain::TEXTURE) {
    this->addRenderable(&mesh);
  }

  void TerrainChunk::init(Engine* engine) {
    GameObject::init(engine);
    mesh.init();
  }
  
  f32 test(Vec2f pos, Vec3f* norm) {
    *norm = Vec3f(0,0,1);
    return 1;
  }

  BoundingObject TerrainChunk::getTightBoundingObject() {
    BoundingObject ret(BOUNDING_GROUND);
    ret.ground.dataAtPoint = test;/*[] (Vec2f pos, Vec3f* norm)->f32 {
      *norm = Vec3f(0,0,1);
      return 1;
      };*/
    return ret;
  }
  
  BoundingObject TerrainChunk::getLooseBoundingObject() {
    return BoundingObject();
  }

  Terrain::Terrain(Engine* _engine) : engine(_engine) {}

  void Terrain::generate(Vec2u size) {
    debugAssert(chunks.size() == 0,
		"Terrain chunk array should be empty before generation");
    for (u32 chunk_x = 0; chunk_x < size.x(); ++chunk_x) {
      for (u32 chunk_y = 0; chunk_y < size.y(); ++chunk_y) {

	Vec3f pos = Vec3f(chunk_x*TERRAIN_CHUNK_SIZE,
			  chunk_y*TERRAIN_CHUNK_SIZE,
			  0);
	chunks.emplace_back(pos);
	TerrainChunk* c = &chunks.back();

        for (u8 x = 0; x < TERRAIN_CHUNK_RES; ++x) {
	  for (u8 y = 0; y < TERRAIN_CHUNK_RES; ++y) {
	    c->mesh.data.push_back(TerrainGenerate::dataAtPos
			     (Vec2f(TERRAIN_CHUNK_STEP*x+pos.x(),TERRAIN_CHUNK_STEP*y+pos.y())));
	  }
	}

	c->init(engine);
	
      }
    }
  }

}
