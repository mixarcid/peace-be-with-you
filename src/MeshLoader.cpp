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

  void loadStaticMesh(FILE* file) {

    uint32_t num_faces = fio::readLittleEndian<uint32_t>(file);
    debugAssert(num_faces > 0,
		"Why are you loading a mesh with no vertices?");

    for (uint32_t face_index = 0;
	 face_index < num_faces; ++face_index) {
      
      Vec3f normal = readVec3f(file);
      Vec3f vert1 = readVec3f(file);
      Vec3f vert2 = readVec3f(file);
      Vec3f vert3 = readVec3f(file);
      log::message("Face: %u", face_index);
      log::message("Normal: " + normal.toString());
      log::message("Vert 1: " + vert1.toString());
      log::message("Vert 2: " + vert1.toString());
      log::message("Vert 3: " + vert1.toString());
      
    }
    
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
      log::message("Mesh: " + mesh_name);
      
      unsigned char mesh_type;
      fread(&mesh_type, sizeof(char), 1, file);
      
      if (mesh_type == PMF_TYPE_STATIC) {
      loadStaticMesh(file);
      } else {
	log::fatalError("Unable to determine type of"
			"mesh %s in model %s",
			mesh_name.c_str(), full_name);
      }
      
      }
  }

}
