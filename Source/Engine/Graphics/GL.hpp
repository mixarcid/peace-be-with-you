#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Vector.hpp"
#include "String.hpp"

NAMESPACE {

  enum GraphicsMode {
    PEACE_GL_TRIANGLES,
    PEACE_GL_POINTS,
    PEACE_GL_LINES
  };

  PEACE_DEFINE_BITFLAGS(WindowFlags, 8,
			WINDOW_NO_FLAGS = 0x00,
			WINDOW_FULLSCREEN = 0x01);
  
  namespace gl {
    
    void init();
    GLFWwindow* createWindow(Vec2s size, String name, WindowFlags flags = WINDOW_NO_FLAGS);
    void checkError();
    void ignoreError(GLenum code);

    /*the drawMode and polygonMode functions
      are for more low-level stuff. To switch 
      between points, triangles, and lines,
      use the graphicsMode functions*/
    void setDrawMode(GLenum mode);
    GLenum getDrawMode();
    void setPolygonMode(GLenum mode);
    GLenum getPolygonMode();

    void setGraphicsMode(GraphicsMode mode);
    GraphicsMode getGraphicsMode();
    
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
