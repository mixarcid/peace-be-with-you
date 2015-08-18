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
    graphics.setCamera(&cam);
    f32 cam_speed = 0.1;
    f32 cam_rot_speed = 0.001;
    Physics phys;

    MeshLoader loader("WoodenBox");
    MeshLoader loader2("Monkey");
    StaticMesh* cube = loader.getStaticMesh("Cube");
    StaticMesh* monk = loader2.getStaticMesh("Suzanne");
    StaticObject cube1(cube, Vec3f(0,7,-2));
    PhysicalObject cube2(cube,
			 Material(1, 0.5),
			 Vec3f(0.5,7,2),
			 Vec3f(0,0,0));
    cube1.rotateAbs(Quaternionf(degreesToRadians(0),
				degreesToRadians(0),
				degreesToRadians(0)));
    cube2.rotateAbs(Quaternionf(degreesToRadians(10),
				degreesToRadians(45),
				degreesToRadians(10)));

    graphics.addNode(&cube1);
    phys.addStaticObject(&cube1);
    //graphics.addNode(&test);
    //phys.addStaticObject(&test);
    graphics.addNode(&cube2);
    phys.addDynamicObject(&cube2);

    Input::addCursorPosCallback
      ([&cam, cam_rot_speed]
       (GLFWwindow* win,
	f64 x, f64 y) {
	
	static f64 prev_x = x;
	static f64 prev_y = y;
	static f32 x_tot = 0;
	static f32 y_tot = 0;

	f64 dx = x - prev_x;
	f64 dy = y - prev_y;
	x_tot += dx;
	y_tot += dy;
	
	Quaternionf q(-y_tot*cam_rot_speed,
		      0,
		      -x_tot*cam_rot_speed);
	cam.rotateAbs(q);
	
	prev_x = x;
	prev_y = y;
	  
      });

    Input::addKeyCallback
      ([&cam,
	cam_speed,
	&cube1]
       (GLFWwindow* win,
	i32 key,
	i32 code,
	i32 act,
	i32 mods) {

	Mat3f coord = cam.getCoord();
	//Log::message("\n" + to_string(coord));
	Vec3f right = coord.col(0);
	Vec3f dir = coord.col(1);
	//Log::message("right: " + to_string(right));
	//Log::message("dir: " + to_string(dir));
	//PEACE_SWAP(right.data[2], right.data[1]);
	//PEACE_SWAP(dir.data[2], dir.data[1]);
	
	switch(key) {
	case GLFW_KEY_W:
	  cam.translateRel(dir*cam_speed);
	  break;
	case GLFW_KEY_S:
	  cam.translateRel(-dir*cam_speed);
	  break;
	case GLFW_KEY_A:
	  cam.translateRel(-right*cam_speed);
	  break;
	case GLFW_KEY_D:
	  cam.translateRel(right*cam_speed);
	  break;
	case GLFW_KEY_SPACE:
	  cube1.rotateRel(Quaternionf(degreesToRadians(2),
				      degreesToRadians(0),
				      degreesToRadians(0)));
	  break;
	}
      });

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
