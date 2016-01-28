#include "Assets.hpp"

NAMESPACE {

  Array<AssetLoaderBase*> AssetLoaderBase::all_loaders;
  
  AssetLoaderBase::AssetLoaderBase() {
    all_loaders.push_back(this);
  }

  void loadAllAssets() {
    for (AssetLoaderBase* l : AssetLoaderBase::all_loaders) {
      l->loadAll();
    }
  }
  
  void freeAllAssets() {}
  
}
