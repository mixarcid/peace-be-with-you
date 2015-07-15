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
    Camera cam(Vec3f(0,0,0),
	       Vec3f(0,0,-1),
	       Vec3f(0,1,0),
	       degreesToRadians(90),
	       1, 100);
    graphics.setCamera(&cam);
    f32 cam_speed = 0.1;
    f32 cam_rot_speed = 0.1;
    Input::addKeyCallback([&cam, cam_speed](GLFWwindow* win, i32 key,
					    i32 code, i32 act, i32 mods) {
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

    MeshLoader loader("Soldier");
    BonedMesh monk = loader.getBonedMesh("Frank");
    /*MeshLoader l2("WoodenBox");
    StaticMesh* cube = l2.getStaticMesh("Cube");
  
    PhysicalObject monk_node(&monk, Material(1, 0.5), Vec3f(0,4,-5),
			     Vec3f(0,-10,0));
    graphics.addNode(&monk_node);
    phys.addDynamicObject(&monk_node);

    StaticObject cube_node(cube, Vec3f(0,-2,-5));
    graphics.addNode(&cube_node);
    phys.addStaticObject(&cube_node);*/

    StaticObject monk_node(&monk, Vec3f(0, 0,-5));
    monk_node.rotateAbs(Quaternionf(degreesToRadians(-90.0f),
				    0,
				    0));
    graphics.addNode(&monk_node);
    phys.addStaticObject(&monk_node);


    Time start, end;
    f32 dt = 0;
    end.makeCurrent();
    start.makeCurrent();
    
    while(!glfwWindowShouldClose(window) && running) {

      end.makeCurrent();
      dt = (f32) (end.getMilliseconds() - start.getMilliseconds())/1000;

      //monk_node.rotateRel(Quaternionf(0,dt,0));

      phys.update(dt);

      //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

      graphics.render(window);
    
      glfwSwapBuffers(window);
      glfwPollEvents();
      gl::checkError();
    
      start = end;
    }
  } catch (FatalError e) {
    Log::error(e.what());
  }

  gl::terminate();
  Log::terminate();
  return EXIT_SUCCESS;
}
