// Link statically with GLEW
#define GLEW_STATIC

#include "VectorMath.h"
#include "VertexObjects.h"
#include "Containers.h"
#include "Graphics.h"
#include "Shader.h"
#include "OnionMan.h"

using namespace peace;

float x = 0, y = 0, z = 0, speed = 0.01;

void keyCallback(GLFWwindow* window, int key, int scancode,
		 int action, int mods) {
  
  switch (key) {
  case GLFW_KEY_ESCAPE:
    glfwDestroyWindow(window);
    break;
  case GLFW_KEY_W:
    y += speed;
    break;
  case GLFW_KEY_A:
    x += speed;
    break;
  case GLFW_KEY_S:
    y -= speed;
    break;
  case GLFW_KEY_D:
    x -= speed;
    break;
  }
}

int main() {

  log::init(NULL);
  graphics::init();

  /* Create a windowed mode window and its OpenGL context */
  GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World",
					NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window,keyCallback);

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  glewInit();

  Shader shade("Default");
  shade.use();

  OnionMan onion;

  while (!glfwWindowShouldClose(window)) {
      // Clear the screen to black
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      onion.render();
      
      // Swap buffers
      glfwSwapBuffers(window);
      glfwPollEvents();
  }
  
  graphics::terminate();
  log::terminate();

  return EXIT_SUCCESS;
}

