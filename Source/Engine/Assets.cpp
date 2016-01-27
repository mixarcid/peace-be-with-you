#include <unistd.h>
#include "Assets.hpp"

NAMESPACE {

  const u32 MAX_DIR_STRING_SIZE = 1024;
  
  void LogCurrentDirectory() {
    char cwd[MAX_DIR_STRING_SIZE];
    debugAssert(getcwd(cwd, sizeof(cwd)) != NULL,
		"Error getting the current directory");
    Log::message("Current directory: %s", cwd);
  }

  Array<function<void(void)>> Asset::loaders;
  Array<function<void(void)>> Asset::freers;

  Asset::Asset(function<void(void)> load_func,
	       function<void(void)> free_func) {
    loaders.push_back(load_func);
    freers.push_back(free_func);
  }

  void Asset::loadAll() {
    for (auto loader : loaders) {
      loader();
    }
    Log::message("Finished loading assets");
  }

  void Asset::freeAll() {
    for (auto freer : freers) {
      freer();
    }
    Log::message("Finished freeing assets");
  }

}
