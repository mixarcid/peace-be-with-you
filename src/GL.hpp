#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Standard.hpp"

NAMESPACE {
  
  namespace gl {
    
    void init();
    void checkError();
    void terminate();
    
  }

}
