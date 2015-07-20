#include "MeshLoader.hpp"
#include "String.hpp"
#include "FileIO.hpp"
#include "FileSystem.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

NAMESPACE {

  const char PMF_VERSION = 0x04;
  const char PMF_TYPE_STATIC_NO_TEXTURE = 0;
  const char PMF_TYPE_STATIC_TEXTURE = 1;
  const char PMF_TYPE_BONED_TEXTURE = 2;

  Vec3f readVec3f(FILE* file) {
    f32 x = fio::readLittleEndian<f32>(file);
    f32 y = fio::readLittleEndian<f32>(file);
    f32 z = fio::readLittleEndian<f32>(file);
    return Vec3f(x,y,z);
  }

  Vec2f readVec2f(FILE* file) {
    f32 x = fio::readLittleEndian<f32>(file);
    f32 y = fio::readLittleEndian<f32>(file);
    return Vec2f(x,y);
  }

  Quaternionf readQuaternionf(FILE* file) {
    f32 w = fio::readLittleEndian<f32>(file);
    f32 x = fio::readLittleEndian<f32>(file);
    f32 y = fio::readLittleEndian<f32>(file);
    f32 z = fio::readLittleEndian<f32>(file);
    return Quaternionf(x,y,z,w);
  }
    
  StaticMesh* loadStaticMesh(FILE* file, Texture* tex) {

    //String tex_name = fio::readString(file);
    //Log::message("Texture name: " + tex_name);

    u32 num_verts = fio::readLittleEndian<u32>(file);
    debugAssert(num_verts > 0,
		"Why are you loading a mesh with no vertices?");
    Array<BasicMeshData> data;
    data.reserve(num_verts);

    //Log::message("#Verts: %u", num_verts);
    for (u32 index = 0; index < num_verts; ++index) {

      Vec3f pos = readVec3f(file);
      Vec3f norm = readVec3f(file);
      Vec2f tex_coord = readVec2f(file);
      tex_coord.y = 1 - tex_coord.y;
      //Log::message("Position: " + pos.toString() + " UV: " + tex_coord.toString());
      data.push_back(BasicMeshData(pos, norm, tex_coord));
      
    }
    
    u32 num_elems = 3*fio::readLittleEndian<u32>(file);
    Array<u32> elems;
    elems.reserve(num_elems);
    debugAssert(num_elems > 0,
		"Why are you loading a mesh with no faces?");
    //Log::message("#Elements: %u", num_elems);

    for (u32 index = 0; index < num_elems; ++index) {
      u32 elem = fio::readLittleEndian<u32>(file);
      //Log::message("Element: %u", elem);
      elems.push_back(elem);
    }

    StaticMesh* ret = new StaticMesh(data, elems, tex);
    ret->init();
    return ret;
  }

  BonedMeshBase* loadBonedMesh(FILE* file, Texture* tex) {

    u32 num_verts = fio::readLittleEndian<u32>(file);
    debugAssert(num_verts > 0,
		"Why are you loading a mesh with no vertices?");
    Array<BasicMeshData> data;
    data.reserve(num_verts);
    Array<BonedMeshData> bone_data;
    bone_data.reserve(num_verts);
    
    //Log::message("#Verts: %u", num_verts);
    for (u32 index = 0; index < num_verts; ++index) {

      Vec3f pos = readVec3f(file);
      Vec3f norm = readVec3f(file);
      Vec2f tex_coord = readVec2f(file);
      tex_coord.y = 1 - tex_coord.y;

      /*Log::message("Pos: " + pos.toString());
      Log::message("Norm: " + norm.toString());
      Log::message("Tex_coord: " + tex_coord.toString());*/

      u8 vert_num_bones = fio::readLittleEndian<u8>(file);
      BonedMeshData d(vert_num_bones);
      //Log::message("#bones: %u", d.num_bones);
      fatalAssert(d.num_bones <= Shader::MAX_BONES_PER_VERTEX,
		  "Too many bones per vertex!");
      
      for (u32 i = 0; i < d.num_bones; ++i) {
	d.indexes[i] = fio::readLittleEndian<u32>(file);\
	//d.indexes[i] = 1u;
	d.weights[i] = fio::readLittleEndian<f32>(file);
      }
      
      data.push_back(BasicMeshData(pos, norm, tex_coord));
      bone_data.push_back(d);
      
    }
    
    u32 num_elems = 3*fio::readLittleEndian<u32>(file);
    debugAssert(num_elems > 0,
		"Why are you loading a mesh with no faces?");
    Array<u32> elems;
    elems.reserve(num_elems);
    //Log::message("#Elements: %u", num_elems);

    for (u32 index = 0; index < num_elems; ++index) {
      u32 elem = fio::readLittleEndian<u32>(file);
      //Log::message("Element: %u", elem);
      elems.push_back(elem);
    }

    u32 num_bones = fio::readLittleEndian<u32>(file);
    Array<Bone> bones;
    bones.reserve(num_bones);

    for (u32 i = 0; i < num_bones; ++i) {
      Quaternionf rot = readQuaternionf(file);
      bones.push_back(Bone(Vec3f(0,0,0),
			   rot));
    }

    u32 num_actions = fio::readLittleEndian<u32>(file);
    HashMap<String, BonedAnimation> actions(num_actions);

    /* for (u32 i = 0; i < num_actions; ++i) {
      
      String name = fio::readString(file);
      u32 num_keyframes= fio::readLittleEndian<u32>(file);
      Array<KeyFrame> keyframes;
      keyframes.reserve(num_keyframes);
      //Log::message(name);
      
      for (u32 i = 0; i < num_keyframes; ++i) {

        f32 time = fio::readLittleEndian<f32>(file);
	KeyFrame frame(time, num_bones);

	for (u32 i = 0; i < num_bones; ++i) {
	  Quaternionf rot = readQuaternionf(file);
	  frame.bones.push_back(Bone(Vec3f(0,0,0),
			       rot));
	}
	keyframes.push_back(frame);
      }
      actions.insert(Pair<String, BonedAnimation>
		     (name,
		      BonedAnimation(keyframes)));
		      }*/

    BonedMeshBase* ret = new BonedMeshBase(data,
					   elems,
					   tex,
					   bone_data,
					   bones,
					   actions);
    ret->init();
    return ret;
  }

  MeshLoader::MeshLoader(String filename) {

    //gl::checkError();
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

    u32 num_meshes = fio::readLittleEndian<u32>(file);
    //Log::message("%u", num_meshes);
    debugAssert(num_meshes > 0,
		"Why are you loading a model with no meshes?");
    
    Texture* model_texture = new Texture();
    model_texture->use();
    model_texture->load(filename, Shader::UNI_TEXTURE);

    for (u32 mesh_index = 0;
	 mesh_index < num_meshes; ++mesh_index) {

      String mesh_name = fio::readString(file);

      //Log::message("Mesh name: " + mesh_name);
      
      unsigned char mesh_type;
      fread(&mesh_type, sizeof(char), 1, file);

      switch(mesh_type) {
      case PMF_TYPE_STATIC_NO_TEXTURE:
	Log::fatalError("PMF type STATIC_NO_TEXTURE is"
			" no longer supported");
	break;
      case PMF_TYPE_STATIC_TEXTURE:
	static_meshes[mesh_name] = loadStaticMesh(file,
						  model_texture);
	break;
      case PMF_TYPE_BONED_TEXTURE:
	boned_meshes[mesh_name] = loadBonedMesh(file,
						model_texture);
	break;
      default:
	  Log::fatalError("Unable to determine type of"
			  " mesh %s in model %s",
			  mesh_name.c_str(), full_name.c_str());
	break;
      }
    }

    fclose(file);
    Log::message("Loaded model %s", filename.c_str());
    
  }

  StaticMesh* MeshLoader::getStaticMesh(String name) {
    StaticMesh* ret = static_meshes[name];
    fatalAssert(ret != NULL,
		"The static mesh \"" + name +
		"\" does not exist in this model");
    return ret;
  }

  BonedMesh MeshLoader::getBonedMesh(String name) {
    BonedMeshBase* base = boned_meshes[name];
    fatalAssert(base != NULL,
		"The boned mesh \"" + name +
		"\" does not exist in this model");
    return BonedMesh(base);
  }

  MeshLoader::~MeshLoader() {
    //printf("????\n");
    for (const auto pair : static_meshes) {
      delete pair.second; //the mesh
    }
    for (const auto pair : boned_meshes) {
      delete pair.second;
    }
  }
}
