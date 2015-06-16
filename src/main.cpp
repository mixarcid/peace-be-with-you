#include "VectorMath.hpp"
#include "Transform.hpp"
#include "VertexObjects.hpp"
#include "Containers.hpp"
#include "GL.hpp"
#include "Shader.hpp"
#include "MeshLoader.hpp"
#include "Time.hpp"
#include "Node.hpp"

using namespace peace;

Transform model;

unsigned int dt = 0;
float trans_speed = 0.005;
float rot_speed = 0.0005;

int win_width = 700;
int win_height = 500;

void windowSize(GLFWwindow* window, int width, int height) {
  win_width = width;
  win_height = height;
  float ratio = width / (float) height;
  glViewport(0, 0, width, height);
  Mat4f proj = Mat4f::perspective(degreesToRadians(45.0f),
			    ratio, 1, 100);

  Shader::UNI_PROJ.registerMat4f(proj);
  Mat4f view = Mat4f::lookAt(Vec3f(0.0f,0.0f,1.5f),
		       Vec3f(0,0,0),
		       Vec3f(0,1,0));
  Shader::UNI_VIEW.registerMat4f(view);
}

void cursorCallBack(GLFWwindow* window, double x, double y) {
  //const Vec3f UP(0,1,0);
  Vec3f mouse;
  mouse.x = x - (win_width/2);
  mouse.z = (win_height/2) - y;
  model.rotateRel(Quaternionf(mouse.z*rot_speed*dt,
			      mouse.x*rot_speed*dt, 0));
  glfwSetCursorPos(window, win_width/2, win_height/2);
}

void keyCallback(GLFWwindow* window, int key, int scancode,
		 int action, int mods) {
  
  switch (key) {
  case GLFW_KEY_ESCAPE:
    glfwDestroyWindow(window);
    break;
  case GLFW_KEY_W:
    model.translateRel(Vec3f(0,0,trans_speed*dt));
    break;
  case GLFW_KEY_A:
    model.translateRel(Vec3f(trans_speed*dt,0,0));
    break;
  case GLFW_KEY_S:
    model.translateRel(Vec3f(0,0,-trans_speed*dt));
    break;
  case GLFW_KEY_D:
    model.translateRel(Vec3f(-trans_speed*dt,0,0));
    break;
  case GLFW_KEY_Q:
    model.translateRel(Vec3f(0,-trans_speed*dt,0));
    break;
  case GLFW_KEY_Z:
    model.translateRel(Vec3f(0,trans_speed*dt,0));
    break;
  }
}

int main() {

  Log::init(NULL);
  SystemManager man({&Log::logger});
  man.start();
  gl::init();

  GLFWwindow* window = glfwCreateWindow(win_width, win_height,
					"Peace", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window,keyCallback);
  glfwSetWindowSizeCallback(window,windowSize);
  glfwSetCursorPosCallback(window,cursorCallBack);
  
  // Initialize GLEW
  glewExperimental = GL_TRUE;
  glewInit();
  
  const unsigned char* version = glGetString(GL_VERSION);
  fatalAssert(version != NULL,
	      "Cannot determine OpenGL version");
  Log::message("Your OpenGL version is %s", version);

  Shader shade("Toon");
  shade.use();

  MeshLoader loader("Monkey");
  StaticMesh* monk = loader.getStaticMesh("Suzanne");
  Vec3f axis(0,0,1);
  
  Node monk_node;
  monk_node.translateAbs(Vec3f(0,0,-5));
  //monk_node.flush();
  monk_node.addRenderable(monk);

  Time start, end;
  end.makeCurrent();

  glEnable(GL_DEPTH_TEST);

  windowSize(window, win_width, win_height);
  while(!glfwWindowShouldClose(window)) {

    end.makeCurrent();
    dt = end.getMilliseconds() - start.getMilliseconds();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Log::message(model.getMat().toString());
    monk_node.render(model.getMat());
      
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
    gl::checkError();
    
    start = end;
  }
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  
  gl::terminate();
  Log::terminate();
  //Texture::terminate();

  return EXIT_SUCCESS;
}
