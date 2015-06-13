#include "Graphics.hpp"

NAMESPACE {

  namespace graphics {

    void init() {
      if (!glfwInit()) {
	log::error("Unable to initialize GLFW");
	glfwTerminate();
	exit(EXIT_FAILURE);
      }
      log::message("Initialized graphics libraries");
    }

    void checkError()  {
      
      GLenum err (glGetError());
      while(err!=GL_NO_ERROR) {
	String error;
	  
	switch(err) {
	case GL_INVALID_OPERATION:
	  error="INVALID_OPERATION";      break;
	case GL_INVALID_ENUM:
	  error="INVALID_ENUM";           break;
	case GL_INVALID_VALUE:
	  error="INVALID_VALUE";          break;
	case GL_OUT_OF_MEMORY:
	  error="OUT_OF_MEMORY";          break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
	  error="INVALID_FRAMEBUFFER_OPERATION";  break;
	}
 
	log::error("GL_" + error);
	err=glGetError();
      }
    }
    
    void terminate() {
      glfwTerminate();
    }
    
  }

}
