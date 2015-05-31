// Link statically with GLEW
#define GLEW_STATIC

// Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "VectorMath.h"
#include "VertexObjects.h"

using namespace peace;

// Shader sources
const GLchar* vertexSource =
  "#version 150 core\n"
  "in vec3 position;"
  "in vec3 color;"
  "out vec3 Color;"
  "void main() {"
  "   Color = color;"
  "   gl_Position = vec4(position, 1.0);"
  "}";
const GLchar* fragmentSource =
  "#version 150 core\n"
  "in vec3 Color;"
  "out vec4 outColor;"
  "void main() {"
  "   outColor = vec4(Color, 1.0);"
  "}";

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
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

  // Create Vertex Array Object
  VAO vao(1);
  vao.use(0);

  // Create a Vertex Buffer Object and copy the vertex data to it
  GLuint vbo;
  glGenBuffers(1, &vbo);

  Array<GLfloat> vertices = {
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-left
    0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Top-right
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom-right
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f  // Bottom-left
  };

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

  // Create an element array
  GLuint ebo;
  glGenBuffers(1, &ebo);

  GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
  };

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  // Create and compile the vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  // Create and compile the fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  // Link the vertex and fragment shader into a shader program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  // Specify the layout of the vertex data
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

  GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

  while (!glfwWindowShouldClose(window))
    {
      // Clear the screen to black
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      // Draw a rectangle from the 2 triangles using 6 indices
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      // Swap buffers
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  glDeleteBuffers(1, &ebo);
  glDeleteBuffers(1, &vbo);

  vao.remove();
  glfwTerminate();
}

