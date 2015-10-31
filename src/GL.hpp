#pragma once

#define GLEW_STATIC
//#define PEACE_GL_USE_POINTS

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Standard.hpp"

NAMESPACE {
  
  namespace gl {
    
    void init();
    void checkError();
    void ignoreError(GLenum code);
    void terminate();
    
  }
}

#ifndef NDEBUG
#define PEACE_GL_CHECK_ERROR do {			\
    GLenum err(glGetError());				\
    while(err!=GL_NO_ERROR) {				\
      String error;					\
      switch(err) {					\
      case GL_INVALID_OPERATION:			\
	error="INVALID_OPERATION";      break;		\
      case GL_INVALID_ENUM:				\
	error="INVALID_ENUM";           break;		\
      case GL_INVALID_VALUE:				\
	error="INVALID_VALUE";          break;		\
      case GL_OUT_OF_MEMORY:				\
	error="OUT_OF_MEMORY";          break;		\
      case GL_INVALID_FRAMEBUFFER_OPERATION:		\
	error="INVALID_FRAMEBUFFER_OPERATION";  break;	\
      }							\
      throw FatalError("GL_" + error +			\
		       " at " __FILE__ ":"		\
		       + to_string(__LINE__));		\
      err=glGetError();					\
    }							\
  } while(0)
#else
#define PEACE_GL_CHECK_ERROR
#endif
