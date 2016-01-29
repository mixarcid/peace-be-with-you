#pragma once

#include "Standard.hpp"
#include "Containers.hpp"

#define DIR_SEP "/"

#define DIR_ASSETS "Assets" DIR_SEP
#define DIR_SHADERS DIR_ASSETS "Shaders" DIR_SEP
#define DIR_TEXTURES DIR_ASSETS "Textures" DIR_SEP
#define DIR_SHADER_HEADER DIR_SHADERS "Header"
#define DIR_MODELS DIR_ASSETS "Models" DIR_SEP

#define DIR_VERT_EXTENSION ".vs"
#define DIR_FRAG_EXTENSION ".fs"
#define DIR_MODEL_EXTENSION ".pmf"
#define DIR_TEXTURE_EXTENSION ".png"

#ifndef N_DEBUG
#define PEACE_LOG_LOADED_ASSETS
#endif

//helpful macro for creating AssetLoaders
#define DEFINE_ASSET_LOADER(T)				\
  AssetLoader<T> T##AssetLoader;

NAMESPACE {

  struct AssetLoaderBase {
    AssetLoaderBase();
    virtual void loadAll() = 0;
    virtual ~AssetLoaderBase() {}
    static Array<AssetLoaderBase*> all_loaders;
  };

  template <typename T>
    T* loadAsset(String name) {
    PEACE_UNIMPLIMENTED(NULL);
  }

  template <typename T>
    struct AssetLoader : AssetLoaderBase {

    virtual void loadAll() {
      loaded_assets.reserve(pending_assets.size());
      for (const auto& item : pending_assets) {
	if (loaded_assets.find(item.first) == loaded_assets.end()) {
	  loadAsset<T>(item.first);
	}
      }
      pending_assets.clear();
    }

    static T* getOrLoad(String name) {
      auto it = loaded_assets.find(name);
      if (it == loaded_assets.end()) {
	return loadAsset<T>(name);
      } else {
	return &it->second;
      }
    }
    
    static HashMap<String, bool> pending_assets;
    static HashMap<String, T> loaded_assets;
    
  };
  
  template<typename T>
    HashMap<String, bool> AssetLoader<T>::pending_assets;
  template<typename T>
    HashMap<String, T> AssetLoader<T>::loaded_assets;

  template <typename T>
    struct Asset {

    String name;
    T* item;

    Asset(String _name) : name(_name), item(NULL) {
      AssetLoader<T>::pending_assets[name] = true;
    }

    T* get() {
      if (!item) {
	auto it = AssetLoader<T>::loaded_assets.find(name);
	debugAssert(it != AssetLoader<T>::loaded_assets.end(),
		    "The Asset %s has not been loaded", name.c_str());
	item = &it->second;
      }
      return item;
    }
  };

  void loadAllAssets();
  void freeAllAssets();
  
}
