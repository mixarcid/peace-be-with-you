#include "Graphics3d.hpp"
#include "Camera.hpp"
#include "Node.hpp"
#include "Input.hpp"

NAMESPACE {

  void Graphics3d::addNode(Node* node) {
    nodes.push_back(node);
  }
  
  void Graphics3d::setCamera(Camera* camera) {
    cam = camera;
    Input::addWindowResizeCallback([this](GLFWwindow* win,
					  int w, int h) {
				     cam->onWindowResize(w,h);
				   });
  }
  
  void Graphics3d::setShader(String name) {
    shade = Shader(name);
    shade.use();
  }

  void Graphics3d::render(GLFWwindow* window) {
    
    debugAssert(cam != NULL,
		"You need to give Graphics3d"
		"a Camera before rendering");
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader::UNI_PROJ.registerMat4f(cam->getProj());
    Shader::UNI_VIEW.registerMat4f(cam->getView());
    Mat4f model = cam->getModel();
    
    for (Node* node : nodes) {
      node->render(model);
    }
    
  }

}
