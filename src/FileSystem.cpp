#include <unistd.h>
#include "FileSystem.hpp"

NAMESPACE {

  const u32 MAX_DIR_STRING_SIZE = 1024;
  
  void LogCurrentDirectory() {
    char cwd[MAX_DIR_STRING_SIZE];
    debugAssert(getcwd(cwd, sizeof(cwd)) != NULL,
		"Error getting the current directory");
    Log::message("Current directory: %s", cwd);
  }

}
