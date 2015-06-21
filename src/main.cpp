#include "Graphics3d.hpp"
#include "Input.hpp"
#include "StaticMesh.hpp"
#include "MeshLoader.hpp"
#include "Node.hpp"
#include "Camera.hpp"
#include "Time.hpp"
#include "Physics.hpp"

using namespace peace;

bool running = true;

void keyCallback(GLFWwindow* window, int key, int scancode,
		 int action, int mods) {
  switch (key) {
  case GLFW_KEY_ESCAPE:
    glfwDestroyWindow(window);
    running = false;
    break;
  }
}

int main() {

  Log::init(NULL);
  SystemManager man({&Log::logger});
  man.start();
  gl::init();

  GLFWwindow* window = glfwCreateWindow(700, 400,
					"Peace", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  Input::init(window);
  Input::addKeyCallback(keyCallback);

  glfwMakeContextCurrent(window);
  
  // Initialize GLEW
  glewExperimental = GL_TRUE;
  glewInit();
  
  const unsigned char* version = glGetString(GL_VERSION);
  fatalAssert(version != NULL,
	      "Cannot determine OpenGL version");
  Log::message("Your OpenGL version is %s", version);

  Graphics3d graphics("Toon");
  //graphics.setShader("Toon");
  Camera cam(Vec3f(0,0,0),
	     Vec3f(0,0,-1),
	     Vec3f(0,1,0),
	     degreesToRadians(90),
	     1, 100);
  graphics.setCamera(&cam);
  float cam_speed = 0.1;
  float cam_rot_speed = 0.1;
  Input::addKeyCallback([&cam, cam_speed](GLFWwindow* win, int key,
					  int code, int act, int mods) {
			  Vec3f axis = Vec3f::cross(cam.dir, cam.up);
			  switch(key) {
			  case GLFW_KEY_W:
			    cam.pos += cam.dir * cam_speed;
			    break;
			  case GLFW_KEY_S:
			    cam.pos -= cam.dir * cam_speed;
			    break;
			  case GLFW_KEY_A:
			    cam.pos -= axis * cam_speed;
			    break;
			  case GLFW_KEY_D:
			    cam.pos += axis * cam_speed;
			    break;
			  }
			});
  Physics phys;

  MeshLoader loader("Monkey");
  StaticMesh* monk = loader.getStaticMesh("Suzanne");
  
  DynamicObject monk_node(1, Vec3f(0,0,-5), Vec3f(0,1,-1));
  monk_node.addRenderable(monk);
  graphics.addNode(&monk_node);
  phys.addDynamicObject(&monk_node);

  Time start, end;
  float dt = 0;
  end.makeCurrent();
  start.makeCurrent();
  
  while(!glfwWindowShouldClose(window) && running) {

    end.makeCurrent();
    dt = (float) (end.getMilliseconds() - start.getMilliseconds())/1000;

    monk_node.rotateRel(Quaternionf(0,dt,0));

    phys.update(dt);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    graphics.render(window);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    gl::checkError();
    
    start = end;
  }
  gl::terminate();
  Log::terminate();

  return EXIT_SUCCESS;
}
