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

void keyCallback(GLFWwindow* window, i32 key, i32 scancode,
		 i32 action, i32 mods) {
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
  
  try {

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(700, 700,
					  "Peace", NULL, NULL);
    if (!window) {
      glfwTerminate();
      return -1;
    }

    Input::init(window);
    Input::addKeyCallback(keyCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    gl::ignoreError(GL_INVALID_ENUM);
  
    const unsigned char* version = glGetString(GL_VERSION);
    fatalAssert(version != NULL,
		"Cannot determine OpenGL version");
    Log::message("Your OpenGL version is %s", version);

    Graphics3d graphics("Toon");
    
    //graphics.setShader("Toon");
    Camera cam(degreesToRadians(60), 1, 50);
    cam.rotateAbs(Quaternionf(degreesToRadians(90.0f),0,0));
    graphics.setCamera(&cam);
    f32 cam_speed = 0.1;
    f32 cam_rot_speed = 0.001;
    Physics phys;

    MeshLoader loader("WoodenBox");

    Input::addCursorPosCallback
      ([&cam, cam_rot_speed]
       (GLFWwindow* win,
	f64 x, f64 y) {
	
	static f64 prev_x = x;
	static f64 prev_y = y;

	f64 dx = x - prev_x;
	f64 dy = y - prev_y;
	
	Quaternionf q(-dy*cam_rot_speed,
		      dx*cam_rot_speed,
		      0);
	cam.rotateRel(q);
	
	prev_x = x;
	prev_y = y;
	  
      });

    Input::addKeyCallback
      ([&cam,
	cam_speed]
       (GLFWwindow* win,
	i32 key,
	i32 code,
	i32 act,
	i32 mods) {
	switch(key) {
	case GLFW_KEY_W:
	  cam.translateRel(cam.getDir()*cam_speed);
	  break;
	case GLFW_KEY_S:
	  cam.translateRel(-cam.getDir()*cam_speed);
	  break;
	case GLFW_KEY_A:
	  cam.translateRel(-cam.getRight()*cam_speed);
	  break;
	case GLFW_KEY_D:
	  cam.translateRel(cam.getRight()*cam_speed);
	  break;
	case GLFW_KEY_SPACE:
	  break;
	}
      });

    StaticMesh* cube = loader.getStaticMesh("Cube");
    StaticObject cube1(cube, Vec3f(0,7,-3));
    PhysicalObject cube2(cube, Material(1, 0.5), Vec3f(1,7,2),
			 Vec3f(0,0,0));
    debugAssert(cube != NULL, "What!");

    graphics.addNode(&cube1);
    phys.addStaticObject(&cube1);
    graphics.addNode(&cube2);
    phys.addDynamicObject(&cube2);

    Time start, end;
    f32 dt = 0;
    end.makeCurrent();
    start.makeCurrent();

    cam.getView();
    
    while(!glfwWindowShouldClose(window) && running) {

      end.makeCurrent();
      dt = (f32) (end.getMilliseconds() - start.getMilliseconds())/1000;

      //sword_node.rotateRel(Quaternionf(0,0,dt));

      phys.update(dt);

      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

      graphics.render(window,dt);
    
      glfwSwapBuffers(window);
      glfwPollEvents();
      gl::checkError();
    
      start = end;
    }
  } catch(Exception& e) {
    Log::error(e.what());
  }

  gl::terminate();
  Log::terminate();
  return EXIT_SUCCESS;
}
