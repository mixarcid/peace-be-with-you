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

  Asset::Asset(function<void(void)> loader) {
    loaders.push_back(loader);
  }

  void Asset::loadAll() {
    for (auto loader : loaders) {
      loader();
    }
    Log::message("Finshed loading assets");
  }

}
