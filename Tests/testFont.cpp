#include <GLFW/glfw3.h>
#include <GL/glc.h>
#include <GL/glu.h>
#include <string>

using namespace std;

string str = "";
int x = 0;
int y = -10;
int z = 0;
int speed = 1;
//const char BS = 0x08;

void charCallback(GLFWwindow* window, unsigned int codepoint) {
  //str += (char) codepoint;
}

void keyCallback(GLFWwindow* window, int key, int scancode,
		 int action, int mods) {
  
  switch (key) {
  case GLFW_KEY_ESCAPE:
    glfwDestroyWindow(window);
    break;
  case GLFW_KEY_BACKSPACE:
    if (str.length() > 0) {
      str.pop_back();
    }
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

int main(void)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSetCharCallback(window,charCallback);
  glfwSetKeyCallback(window,keyCallback);

  GLint ctx, myFont;

  // Set up and initialize GLC
  ctx = glcGenContext();
  glcContext(ctx);
  glcAppendCatalog("quicksand");

  // Create a font "Palatino Bold"
  myFont = glcGenFontID();
  glcNewFontFromFamily(myFont, "Quicksand");
  glcFontFace(myFont, "Regular");
  glcFont(myFont);

  
  // Render the text at a size of 100 points
  glcScale(20.f, 20.f);

  glEnable(GL_DEPTH_TEST);
  
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      float ratio;
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
      ratio = width / (float) height;
      glViewport(0, 0, width, height);

      //2d stuff:
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0, width, 0, height, 1.f, -1.f);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      
      glRasterPos2f(50.f, 50.f);
      glcRenderString(str.c_str());

      //3d stuff:
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(90, ratio, 0.1, 100);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Render "Hello world!"
      glTranslatef(x,z,y);
      glColor3f(1.f, 0.f, 0.f);
      glBegin(GL_TRIANGLES);
      glVertex3f(0,0,0);
      glVertex3f(1,1,0);
      glVertex3f(1,0,0);
      glEnd();

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

  glfwTerminate();
  return 0;
}
