#include "GL.hpp"

NAMESPACE {

  namespace gl {

    void init() {
      if (!glfwInit()) {
	Log::error("Unable to initialize GLFW");
	glfwTerminate();
	exit(EXIT_FAILURE);
      }
      Log::message("Initialized graphics libraries");
    }

    GLFWwindow* createWindow(Vec2s size, String name, WindowFlags flags) {
      
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
      
      GLFWwindow* window = glfwCreateWindow
	(size.x(), size.y(),
	 name.c_str(), flags & WINDOW_FULLSCREEN ? glfwGetPrimaryMonitor() : NULL, NULL);
      if (!window) {
	Log::fatalError("Failed to create GLFWwindow");
      }

      //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      glfwMakeContextCurrent(window);
      
      // Initialize GLEW
      glewExperimental = GL_TRUE;
      glewInit();
      gl::ignoreError(GL_INVALID_ENUM);

      return window;
    };

    void checkError()  {
      
      GLenum err(glGetError());
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
 
	Log::error("GL_" + error);
	err=glGetError();
      }
    }

    void ignoreError(GLenum code) {
      GLenum err(glGetError());
      if (err == code) return;
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
 
	Log::error("GL_" + error);
	err=glGetError();
      }
    }

    static GLenum draw_mode = GL_TRIANGLES;
    void setDrawMode(GLenum mode) {
      draw_mode = mode;
    }
    
    GLenum getDrawMode() {
      return draw_mode;
    }
    
    void terminate() {
      glfwTerminate();
    }

    static GLenum polygon_mode = GL_FILL;
    void setPolygonMode(GLenum mode) {
      if (mode != polygon_mode) {
	polygon_mode = mode;
	glPolygonMode(GL_FRONT_AND_BACK, mode);
      }
    }

    GLenum getPolygonMode() {
      return polygon_mode;
    }

    static GraphicsMode graphics_mode = PEACE_GL_TRIANGLES;
    void setGraphicsMode(GraphicsMode mode) {
      if (graphics_mode != mode) {
	graphics_mode = mode;
	switch(graphics_mode) {
	case PEACE_GL_TRIANGLES:
	  gl::setDrawMode(GL_TRIANGLES);
	  gl::setPolygonMode(GL_FILL);
	  break;
	case PEACE_GL_LINES:
	  gl::setDrawMode(GL_TRIANGLES);
	  gl::setPolygonMode(GL_LINE);
	  break;
	case PEACE_GL_POINTS:
	  gl::setDrawMode(GL_POINTS);
	  gl::setPolygonMode(GL_FILL);
	  break;
	}
      }
    }

    GraphicsMode getGraphicsMode() {
      return graphics_mode;
    }
    
  }

}
