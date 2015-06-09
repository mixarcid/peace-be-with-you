#include "MeshLoader.h"
#include "String.h"
#include "FileIO.h"
#include "FileSystem.h"

NAMESPACE {

  const char PMF_VERSION = 0x01;
  const char PMF_TYPE_STATIC = 0;

  Vec3f readVec3f(FILE* file) {
    float x = fio::readLittleEndian<float>(file);
    float y = fio::readLittleEndian<float>(file);
    float z = fio::readLittleEndian<float>(file);
    return Vec3f(x,y,z);
  }

  void loadStaticMesh(FILE* file, StaticMesh* mesh) {

    Array<StaticMeshData> data;
    Array<GLuint> elems;

    uint32_t num_verts = fio::readLittleEndian<uint32_t>(file);
    debugAssert(num_verts > 0,
		"Why are you loading a mesh with no vertices?");

    for (uint32_t index = 0; index < num_verts; ++index) {

      Vec3f pos = readVec3f(file);
      Vec3f norm = readVec3f(file);
      log::message("Position: " + pos.toString());
      log::message("Normal: " + norm.toString());
      data.push_back(StaticMeshData(pos, norm));
      /*data.push_back(StaticMeshData(readVec3f(file),
	readVec3f(file)));*/
      
    }
    
    uint32_t num_elems = fio::readLittleEndian<uint32_t>(file);
    debugAssert(num_elems > 0,
		"Why are you loading a mesh with no faces?");
    log::message("#Elements: %u", num_elems);

    for (uint32_t index = 0; index < num_elems; ++index) {
      uint32_t elem = fio::readLittleEndian<uint32_t>(file);
      log::message("Element: %u", elem);
      elems.push_back(elem);
    }

    mesh->init(data, elems);
    
  }

  MeshLoader::MeshLoader(String filename) {

    const char* full_name = (DIR_MODELS + filename).c_str();
    FILE* file = fopen(full_name, "rb");
    fatalAssert(file != NULL,
		"Unable to open model %s", full_name);
    
    char sig[3];
    fread(sig, sizeof(char), 3, file);
    fatalAssert(sig[0] == 'P' && sig[1] == 'M' && sig[2] == 'F',
		"Incorrect signature for model %s", full_name);
    
    char ver;
    fread(&ver, sizeof(char), 1, file);
    fatalAssert(ver == PMF_VERSION,
		"Incorrect PMF version for model %s", full_name);

    uint32_t num_meshes = fio::readLittleEndian<uint32_t>(file);
    //log::message("%u", num_meshes);
    debugAssert(num_meshes > 0,
		"Why are you loading a model with no meshes?");

    for (uint32_t mesh_index = 0;
	 mesh_index < num_meshes; ++mesh_index) {

      String mesh_name = fio::readString(file);
      
      unsigned char mesh_type;
      fread(&mesh_type, sizeof(char), 1, file);
      
      if (mesh_type == PMF_TYPE_STATIC) {
	loadStaticMesh(file, &static_meshes[mesh_name]);
      } else {
	log::fatalError("Unable to determine type of"
			"mesh %s in model %s",
			mesh_name.c_str(), full_name);
      }
      
    }
  }

  StaticMesh* MeshLoader::getStaticMesh(String name) {
    return &static_meshes[name];
  }

}
