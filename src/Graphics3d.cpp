#include "Graphics3d.hpp"
#include "Camera.hpp"
#include "Node.hpp"
#include "Input.hpp"

NAMESPACE {

  void Graphics3d::addNode(Node* node) {
    nodes.push_back(node);
  }

  void Graphics3d::addDirLight(DirLight* light) {
    debugAssert(dir_lights.size() <
		Shader::MAX_DIR_LIGHTS,
		"Trying to add too many directional "
		"lights to scene");
    dir_lights.push_back(light);
  }

  void Graphics3d::setAmbient(f32 ambient_light) {
    ambient = ambient_light;
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

    DirLight dir_light_arr[Shader::MAX_DIR_LIGHTS];
    /*memset(dir_light_arr,
	   0,
	   Shader::MAX_DIR_LIGHTS*sizeof(DirLight));*/
    for (u8 i=0; i<dir_lights.size(); ++i) {
      dir_light_arr[i] = *dir_lights[i];
      //Log::message(to_string(dir_light_arr[i].color));
    }
    Shader::UNI_DIR_LIGHTS
      .registerArray(dir_light_arr,
		     Shader::MAX_DIR_LIGHTS);
    Shader::UNI_AMBIENT.registerVal(ambient);

    Mat4f view_proj = cam->getProj()*cam->getView();
    Shader::UNI_VIEW_PROJ.registerVal(view_proj);
    Mat4f model;
    RenderContext c(dt);
    
    for (Node* node : nodes) {
      node->render(c, model);
    }
    
  }
}
