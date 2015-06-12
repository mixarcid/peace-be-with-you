#include "MeshLoader.hpp"
#include "String.hpp"
#include "FileIO.hpp"
#include "FileSystem.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

NAMESPACE {

  const char PMF_VERSION = 0x03;
  const char PMF_TYPE_STATIC_NO_TEXTURE = 0;
  const char PMF_TYPE_STATIC_TEXTURE = 1;

  Vec3f readVec3f(FILE* file) {
    float x = fio::readLittleEndian<float>(file);
    float y = fio::readLittleEndian<float>(file);
    float z = fio::readLittleEndian<float>(file);
    return Vec3f(x,y,z);
  }

  Vec2f readVec2f(FILE* file) {
    float x = fio::readLittleEndian<float>(file);
    float y = fio::readLittleEndian<float>(file);
    return Vec2f(x,y);
  }
    
  StaticMesh* loadStaticMesh(FILE* file, Texture* tex) {

    Array<StaticMeshData> data;
    Array<GLuint> elems;

    //String tex_name = fio::readString(file);
    //log::message("Texture name: " + tex_name);

    uint32_t num_verts = fio::readLittleEndian<uint32_t>(file);
    debugAssert(num_verts > 0,
		"Why are you loading a mesh with no vertices?");

    //log::message("#Verts: %u", num_verts);
    for (uint32_t index = 0; index < num_verts; ++index) {

      Vec3f pos = readVec3f(file);
      Vec3f norm = readVec3f(file);
      Vec2f tex_coord = readVec2f(file);
      tex_coord.y = 1 - tex_coord.y;
      //log::message("Position: " + pos.toString());
      //log::message("Normal: " + norm.toString());
      data.push_back(StaticMeshData(pos, norm, tex_coord));
      /*data.push_back(StaticMeshData(readVec3f(file),
	readVec3f(file)));*/
      
    }
    
    uint32_t num_elems = 3*fio::readLittleEndian<uint32_t>(file);
    debugAssert(num_elems > 0,
		"Why are you loading a mesh with no faces?");
    //log::message("#Elements: %u", num_elems);

    for (uint32_t index = 0; index < num_elems; ++index) {
      uint32_t elem = fio::readLittleEndian<uint32_t>(file);
      //log::message("Element: %u", elem);
      elems.push_back(elem);
    }

    return new StaticMesh(data, elems, tex);
    
  }

  MeshLoader::MeshLoader(String filename) {

    String full_name = (DIR_MODELS + filename
			+ DIR_MODEL_EXTENSION);
    
    FILE* file = fopen(full_name.c_str(), "rb");
    fatalAssert(file != NULL,
		"Unable to open model %s", full_name.c_str());
    
    char sig[3];
    fread(sig, sizeof(char), 3, file);
    fatalAssert(sig[0] == 'P' && sig[1] == 'M' && sig[2] == 'F',
		"Incorrect signature for model %s",
		full_name.c_str());
    
    char ver;
    fread(&ver, sizeof(char), 1, file);
    fatalAssert(ver == PMF_VERSION,
		"Incorrect PMF version for model %s",
		full_name.c_str());

    uint32_t num_meshes = fio::readLittleEndian<uint32_t>(file);
    //log::message("%u", num_meshes);
    debugAssert(num_meshes > 0,
		"Why are you loading a model with no meshes?");

    Texture model_texture = Texture::getTexture();
    model_texture.use();
    model_texture.load(filename, Shader::UNI_TEXTURE);

    for (uint32_t mesh_index = 0;
	 mesh_index < num_meshes; ++mesh_index) {

      String mesh_name = fio::readString(file);
      //log::message("Mesh name: " + mesh_name);
      
      unsigned char mesh_type;
      fread(&mesh_type, sizeof(char), 1, file);

      switch(mesh_type) {
      case PMF_TYPE_STATIC_NO_TEXTURE:
	log::fatalError("PMF type STATIC_NO_TEXTURE is"
			" no longer supported");
	break;
      case PMF_TYPE_STATIC_TEXTURE:
	static_meshes[mesh_name] = loadStaticMesh(file,
						  &model_texture);
	break;
      default:
	  log::fatalError("Unable to determine type of"
			  "mesh %s in model %s",
			  mesh_name.c_str(), full_name.c_str());
	break;
      }
    }

    fclose(file);
    log::message("Loaded model %s", filename.c_str());
    
  }

  StaticMesh* MeshLoader::getStaticMesh(String name) {
    StaticMesh* ret = static_meshes[name];
    fatalAssert(ret != NULL,
		"The static mesh " + name +
		" does not exist in this model");
    return ret;
  }

  MeshLoader::~MeshLoader() {
    for (const auto pair : static_meshes) {
      delete pair.second; //the mesh
    }
  }
}
