#include "MeshLoader.hpp"
#include "String.hpp"
#include "FileIO.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Assets.hpp"
#include "VectorIO.hpp"

NAMESPACE {

  DEFINE_ASSET_LOADER(StaticMesh);
  DEFINE_ASSET_LOADER(BonedMeshBase);

  //PMF header stuff
  const static char PMF_VERSION = 0x04;
  
  enum PMFMeshType : char {
    PMF_TYPE_STATIC_NO_TEXTURE = 0,
      PMF_TYPE_STATIC_TEXTURE = 1,
      PMF_TYPE_BONED_TEXTURE = 2
      };

  void loadStaticMesh(String name, FILE* file, Texture tex) {
 
    auto inserted = AssetLoader<StaticMesh>::
      loaded_assets.emplace(name, tex);
      
    debugAssert(inserted.second,
		"There was a problem inserting the StaticMesh %s"
		" into the HashMap",
		name.c_str());
    
    StaticMesh& mesh = inserted.first->second;
    
    u32 num_verts = fio::readLittleEndian<u32>(file);
    fatalAssert(num_verts > 0,
		"Why are you loading a mesh with no vertices?");
    Array<BasicMeshData> mesh_data;
    mesh_data.reserve(num_verts);

    for (u32 index = 0; index < num_verts; ++index) {

      Vec3f pos;
      load(file, &pos);
      Vec3f norm;
      load(file, &norm);
      Vec2f tex_coord;
      load(file, &tex_coord);
      tex_coord.y() = 1 - tex_coord.y();
      mesh_data.push_back(BasicMeshData(pos, norm, tex_coord));
      
    }
    
    u32 num_elems = 3*fio::readLittleEndian<u32>(file);
    Array<u32> mesh_elements;
    mesh_elements.reserve(num_elems);
    fatalAssert(num_elems > 0,
		"Why are you loading a mesh with no faces?");

    for (u32 index = 0; index < num_elems; ++index) {
      u32 elem = fio::readLittleEndian<u32>(file);
      mesh_elements.push_back(elem);
    }

    mesh.init(mesh_data, mesh_elements);
#ifdef PEACE_LOG_LOADED_ASSETS
    Log::message("Loaded StaticMesh %s", name.c_str());
#endif
  }
  
  void loadBonedMesh(String name, FILE* file, Texture tex) {

    PEACE_GL_CHECK_ERROR;

    auto inserted = AssetLoader<BonedMeshBase>::
      loaded_assets.emplace(name, tex);
    
    debugAssert(inserted.second,
		"There was a problem loading the BonedMesh %s"
		" into the HashMap",
		name.c_str());

    BonedMeshBase& mesh = inserted.first->second;
    
    u32 num_verts = fio::readLittleEndian<u32>(file);
    fatalAssert(num_verts > 0,
		"Why are you loading a mesh with no vertices?");
    Array<BasicMeshData> mesh_data;
    Array<BonedMeshData> bone_data;
    mesh_data.reserve(num_verts);
    bone_data.reserve(num_verts);
    
    for (u32 index = 0; index < num_verts; ++index) {

      Vec3f pos;
      load(file, &pos);
      Vec3f norm;
      load(file, &norm);
      Vec2f tex_coord;
      load(file, &tex_coord);
      tex_coord.y() = 1 - tex_coord.y();
      
      u8 vert_num_bones = fio::readLittleEndian<u8>(file);
      BonedMeshData d;
      
      fatalAssert(vert_num_bones <= Shader::MAX_BONES_PER_VERTEX,
		  "Too many bones per vertex!");
      
      for (u32 i = 0; i < vert_num_bones; ++i) {
	d.indexes[i] = fio::readLittleEndian<u32>(file);
	d.weights[i] = fio::readLittleEndian<f32>(file);
      }
      
      mesh_data.push_back(BasicMeshData(pos, norm, tex_coord));
      bone_data.push_back(d);
      
    }
    
    u32 num_elems = 3*fio::readLittleEndian<u32>(file);
    fatalAssert(num_elems > 0,
		"Why are you loading a mesh with no faces?");
    Array<u32> mesh_elements;
    mesh_elements.reserve(num_elems);

    for (u32 index = 0; index < num_elems; ++index) {
      u32 elem = fio::readLittleEndian<u32>(file);
      mesh_elements.push_back(elem);
    }

    u32 num_bones = fio::readLittleEndian<u32>(file);
    mesh.bones.reserve(num_bones);
    
    for (u32 i = 0; i < num_bones; ++i) {
      Vec3f trans;
      load(file, &trans);
      Quaternionf rot;
      load(file, &rot);
      mesh.bones.push_back(Bone(trans, rot));
    }

    u32 num_actions = fio::readLittleEndian<u32>(file);
    mesh.animations.reserve(num_actions);

    for (u32 i = 0; i < num_actions; ++i) {
      
      String name;
      load(file, &name);
      auto inserted = mesh.animations.insert(Pair<String, BonedAnimationBase>
					     (name, BonedAnimationBase()));
      BonedAnimationBase& base = inserted.first->second;
      
      u32 num_keyframes= fio::readLittleEndian<u32>(file);
      fatalAssert(num_keyframes > 1,
		  "The animation %s has less than two keyframes", name.c_str());
      base.keyframes.reserve(num_keyframes);
      
      for (u32 j = 0; j < num_keyframes; ++j) {

        f32 time = fio::readLittleEndian<f32>(file);
	KeyFrame frame(time, num_bones);

	for (u32 k = 0; k < num_bones; ++k) {
	  Vec3f trans;
	  load(file, &trans);
	  Quaternionf rot;
	  load(file, &rot);
	  frame.bones.push_back(Bone(trans,
				     rot));
	}
	
	base.keyframes.push_back(frame);
      }
    }

    PEACE_GL_CHECK_ERROR;
    mesh.init(mesh_data, bone_data, mesh_elements);
    PEACE_GL_CHECK_ERROR;
    
#ifdef PEACE_LOG_LOADED_ASSETS
    Log::message("Loaded BonedMesh %s", name.c_str());
#endif
    }

  void loadPMF(String filename) {
    PEACE_GL_CHECK_ERROR;
    String full_name = (DIR_MODELS + filename
			+ DIR_MODEL_EXTENSION);
    
    FILE* file = fopen(full_name.c_str(), "rb");
    fatalAssert(file != NULL,
		"Unable to open model %s", full_name.c_str());
    
    char sig[3];
    fread(sig, sizeof(char), 3, file);
    fatalAssert(sig[0] == 'P' && sig[1] == 'M' && sig[2] == 'F',
		"Incorrect file signature for model %s",
		full_name.c_str());
    
    char ver;
    fread(&ver, sizeof(char), 1, file);
    fatalAssert(ver == PMF_VERSION,
		"Incorrect PMF version for model %s",
		full_name.c_str());

    u32 num_meshes = fio::readLittleEndian<u32>(file);
    //Log::message("%u", num_meshes);
    fatalAssert(num_meshes > 0,
		"Why are you loading a model with no meshes?");

    Texture* model_texture = AssetLoader<Texture>::getOrLoad(filename);
    PEACE_GL_CHECK_ERROR;
    for (u32 mesh_index = 0;
	 mesh_index < num_meshes; ++mesh_index) {

      String mesh_name;
      load(file, &mesh_name);
      String final_name = filename + ":" + mesh_name;

      //Log::message("Mesh name: " + mesh_name);
      
      unsigned char mesh_type;
      fread(&mesh_type, sizeof(char), 1, file);
      
      PEACE_GL_CHECK_ERROR;
      
      switch(mesh_type) {
      case PMF_TYPE_STATIC_NO_TEXTURE:
	Log::fatalError("PMF type STATIC_NO_TEXTURE is"
			" no longer supported");
	break;
      case PMF_TYPE_STATIC_TEXTURE:
	loadStaticMesh(final_name, file, *model_texture);
	break;
      case PMF_TYPE_BONED_TEXTURE:
	loadBonedMesh(final_name, file, *model_texture);
	break;
      default:
	Log::fatalError("Unable to determine type of"
			" mesh %s in model %s",
			mesh_name.c_str(), full_name.c_str());
	break;
      }
    }

    fclose(file);
    
  }

  void loadModelFromMeshName(String mesh_name) {
    size_t index = mesh_name.find(":");
    debugAssert(index != String::npos,
		"Your mesh name is incorrectly formatted; "
		"all mesh names must have the format "
		"\"Filename:Meshname\"");
    loadPMF(mesh_name.substr(0,index));
  }

  template<>
    StaticMesh* loadAsset<StaticMesh>(String name) {
    loadModelFromMeshName(name);
    auto found = AssetLoader<StaticMesh>::loaded_assets.find(name);
    debugAssert(found != AssetLoader<StaticMesh>::loaded_assets.end(),
		"Something went wrong while loading the StaticMesh %s. "
		"Perhaps it doesn't exist?",
		name.c_str());
    return &found->second;
  }
 
  template<>
    BonedMeshBase* loadAsset<BonedMeshBase>(String name) {
    loadModelFromMeshName(name);
    auto found = AssetLoader<BonedMeshBase>::loaded_assets.find(name);
    debugAssert(found != AssetLoader<BonedMeshBase>::loaded_assets.end(),
		"Something went wrong while loading the BonedMesh %s. "
		"Perhaps it doesn't exist?",
		name.c_str());
    return &found->second;
  }
  
}
