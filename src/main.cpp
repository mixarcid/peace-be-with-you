#include "Graphics.hpp"
#include "GUIWidgets.hpp"
#include "Input.hpp"
#include "StaticMesh.hpp"
#include "MeshLoader.hpp"
#include "Node.hpp"
#include "Camera.hpp"
#include "Time.hpp"
#include "Physics.hpp"
#include "Assets.hpp"

using namespace peace;

bool running = true;

void keyCallback(GLFWwindow* window, i32 key, i32 scancode,
		 i32 action, i32 mods) {
  switch (key) {
  case GLFW_KEY_ESCAPE:
    glfwDestroyWindow(window);
    running = false;
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
    Input::setManager("Main");
    Input::addFlags(INPUT_CURSOR_DISABLED);
    Input::addKeyCallback(keyCallback);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    gl::ignoreError(GL_INVALID_ENUM);
  
    const unsigned char* version = glGetString(GL_VERSION);
    fatalAssert(version != NULL,
		"Cannot determine OpenGL version");
    Log::message("Your OpenGL version is %s", version);
    
    Graphics graphics("Toon");
    PEACE_GL_CHECK_ERROR;
    Asset::loadAll();
    PEACE_GL_CHECK_ERROR;
    DirLight light(Vec3f(0,0,1),
		   Vec3f(1,1,1));
    graphics.addDirLight(&light);
    graphics.setAmbient(0);

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
    PhysicalObject cube2(monk,
			 Material(1, 0.5),
			 Vec3f(0.5,7,2),
			 Vec3f(0,0,0));
    cube1.rotateAbs(Quaternionf(degreesToRadians(0),
				degreesToRadians(10),
				degreesToRadians(0)));
    graphics.addNode(&cube1);
    phys.addStaticObject(&cube1);
    graphics.addNode(&cube2);
    phys.addDynamicObject(&cube2);
    MeshLoader loader3("SubjectB");
    BonedMesh subject = loader3.getBonedMesh("Subject");
    StaticObject sub(&subject, Vec3f(0,10,0));
    graphics.addNode(&sub);
    phys.addStaticObject(&sub);

    Texture button_tex;
    //button_tex.use();
    button_tex.load("DefaultButton", Shader::UNI_TEXTURE);
    GUITextBox text(Vec2s(0,0),
		    "Peace Be With You",
		    100,
		    {Vec2f(0,0), Vec2f(0,1), Vec2f(1,1), Vec2f(1,0)},
		    &button_tex,
		    GUI_FLOAT_CENTER,
		    0,
		    Vec4f(0.5,0.5,0,1),
		    GUITextProperties
		    (GUI_TEXT_STYLE_ITALIC,
		     GUI_TEXT_ALIGN_CENTER));
    graphics.addGUINode(&text);

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

    Input::addCharCallback
      ([&text]
       (GLFWwindow* win, u32 code) {
	static String str;
	str += (char) code;
	text.setContents(str);
      });

    Input::addKeyCallback
      ([&cam,
	cam_speed,
	&subject]
       (GLFWwindow* win,
	i32 key,
	i32 code,
	i32 act,
	i32 mods) {

	Mat3f coord = cam.getCoord();
	Vec3f right = coord.col(0);
	Vec3f dir = coord.col(1);
	
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
	  subject.startAnimation("Walk");
	  break;
	}
      });

    Time start, end;
    f32 dt = 0;
    end.makeCurrent();
    start.makeCurrent();

    while(!glfwWindowShouldClose(window) && running) {

      end.makeCurrent();
      dt = (f32) (end.getMilliseconds() - start.getMilliseconds())/1000;

      //sword_node.rotateRel(Quaternionf(0,0,dt));

      phys.update(dt);
      light.dir = Quaternionf(0.02,0,0)*light.dir;
      light.dir.normalize();

      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

      graphics.render(window,dt);
    
      glfwSwapBuffers(window);
      glfwPollEvents();
      start = end;
      gl::checkError();
    }
    //delete text;
  } catch (FatalError& e) {
    Log::error(e.msg +
	       "\nStack trace for Exception: \n"
	       + e.backtrace);
    
  }
  
  Asset::freeAll();
  gl::terminate();
  Log::terminate();
  return EXIT_SUCCESS;
}
