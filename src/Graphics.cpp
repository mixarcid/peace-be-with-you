#include "Graphics.h"

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

    void terminate() {
      glfwTerminate();
    }
    
  }

}
