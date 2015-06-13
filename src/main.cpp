// Link statically with GLEW
#define GLEW_STATIC

#include "VectorMath.hpp"
#include "Transform.hpp"
#include "VertexObjects.hpp"
#include "Containers.hpp"
#include "Graphics.hpp"
#include "Shader.hpp"
#include "MeshLoader.hpp"
#include "Time.hpp"

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
			    ratio, 1, 10);

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
  /*Vec3f axis = Vec3f::cross(UP, mouse);
    axis.makeUnit();
    float angle = degreesToRadians(dt*rot_speed);//-sin(axis.abs())*dt*rot_speed;*/
  model.rotate(Vec3f(0,1,0), mouse.x*rot_speed*dt);
  model.rotate(Vec3f(1,0,0), mouse.z*rot_speed*dt);
  glfwSetCursorPos(window, win_width/2, win_height/2);
}

void keyCallback(GLFWwindow* window, int key, int scancode,
		 int action, int mods) {
  
  switch (key) {
  case GLFW_KEY_ESCAPE:
    glfwDestroyWindow(window);
    break;
  case GLFW_KEY_W:
    model.translate(Vec3f(0,0,trans_speed*dt));
    break;
  case GLFW_KEY_A:
    model.translate(Vec3f(trans_speed*dt,0,0));
    break;
  case GLFW_KEY_S:
    model.translate(Vec3f(0,0,-trans_speed*dt));
    break;
  case GLFW_KEY_D:
    model.translate(Vec3f(-trans_speed*dt,0,0));
    break;
  case GLFW_KEY_Q:
    model.translate(Vec3f(0,-trans_speed*dt,0));
    break;
  case GLFW_KEY_Z:
    model.translate(Vec3f(0,trans_speed*dt,0));
    break;
  }
}

int main() {

  log::init(NULL);
  graphics::init();

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
  log::message("Your OpenGL version is %s", version);
  //delete version;
  
  VAO::init();
  Texture::init();

  Shader shade("Toon");
  shade.use();

  MeshLoader loader("Monkey");
  MeshLoader loader2("WoodenBox");
  StaticMesh* cube = loader2.getStaticMesh("Cube");
  StaticMesh* frank = loader.getStaticMesh("Suzanne");
  Vec3f axis(0,0,1);
  
  Transform trans1;
  trans1.setTranslateAbs(Vec3f(0,0,-5));
  //trans1.setRotateAbs(Vec3f(0,1,0), degreesToRadians(30.0f));
  trans1.flush();
    
  Transform trans2;
  trans2.setTranslateAbs(Vec3f(5,0,-5));
  //trans2.setScaleAbs(Vec3f(1,1.5,1));
  //trans2.setRotateAbs(Vec3f(0,1,0), degreesToRadians(90.0f));
  trans2.flush();

  Time start, end;
  end.makeCurrent();

  glEnable(GL_DEPTH_TEST);

  windowSize(window, win_width, win_height);
  while(!glfwWindowShouldClose(window)) {

    end.makeCurrent();
    dt = end.getMilliseconds() - start.getMilliseconds();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Transform::combine(trans1, model).use();
    cube->render();
    Transform::combine(trans2, model).use();
    frank->render();
      
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
    graphics::checkError();
    
    start = end;
  }
  
  graphics::terminate();
  log::terminate();
  VAO::terminate();
  Texture::terminate();

  return EXIT_SUCCESS;
}
