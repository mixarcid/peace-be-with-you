#include "Graphics3d.hpp"
#include "Camera.hpp"
#include "Node.hpp"
#include "Input.hpp"

NAMESPACE {

  Graphics3d::Graphics3d(String shader_name) : shade(shader_name) {
    shade.use();
  }

  void Graphics3d::addNode(Node* node) {
    nodes.push_back(node);
  }
  
  void Graphics3d::setCamera(Camera* camera) {
    cam = camera;
    Input::addWindowResizeCallback([this](GLFWwindow* win,
					  i32 w, i32 h) {
				     cam->onWindowResize(w,h);
				   });
    i32 w, h;
    Input::getWindowSize(&w,&h);
    cam->onWindowResize(w,h);
  }
  
  /*void Graphics3d::setShader(String name) {
    shade = Shader(name);
    shade.use();
  }*/

  void Graphics3d::render(GLFWwindow* window,f32 dt) {
    
    debugAssert(cam != NULL,
		"You need to give Graphics3d"
		"a Camera before rendering");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shade.use();

    Shader::UNI_PROJ.registerMat4f(cam->getProj());
    Shader::UNI_VIEW.registerMat4f(cam->getView());
    Mat4f model;
    RenderContext c(dt);
    
    for (Node* node : nodes) {
      node->render(c, model);
    }
    
  }

}
