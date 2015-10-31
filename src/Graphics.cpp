#include "Graphics.hpp"
#include "Camera.hpp"
#include "Node.hpp"
#include "GUI.hpp"
#include "Input.hpp"

NAMESPACE {

  Vec2f Graphics::window_size;

  Graphics::Graphics(String shader_name) : shade(shader_name) {
    shade.use();
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifdef PEACE_GL_USE_POINTS
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    i32 w, h;
    Input::getWindowSize(&w, &h);
    window_size.data[0] = w;
    window_size.data[1] = h;
  }

  ArrayHandle Graphics::addNode(Node* node) {
    return nodes.push_back(node);
  }

  ArrayHandle Graphics::addGUINode(GUINode* node) {
    ArrayHandle ret = nodes_2d.insertSorted(node,
					    GUINode::compare);
    /*for (GUINode* node : nodes_2d) {
      Log::message("z: %d", node->z_val);
      }*/
    return ret;
  }

  ArrayHandle Graphics::addDirLight(DirLight* light) {
    debugAssert(dir_lights.size() <
		Shader::MAX_DIR_LIGHTS,
		"Trying to add too many directional "
		"lights to scene");
    return dir_lights.push_back(light);
  }

  void Graphics::removeNode(ArrayHandle h) {
    nodes.removeAndReplace(h);
  }
  void Graphics::removeGUINode(ArrayHandle h) {
    nodes_2d.removeAndReplace(h);
  }
  void Graphics::removeDirLight(ArrayHandle h) {
    dir_lights.removeAndReplace(h);
  }

  void Graphics::setAmbient(f32 ambient_light) {
    ambient = ambient_light;
  }
  
  void Graphics::setCamera(Camera* camera) {
    cam = camera;
    Input::addWindowResizeCallback
      ([this]
       (GLFWwindow* win,
	i32 w, i32 h) {
	cam->onWindowResize(w,h);
	window_size = Vec2f(w,h);
      });
    cam->onWindowResize(window_size.x(),
			window_size.y());
  }
  
  /*void Graphics::setShader(String name) {
    shade = Shader(name);
    shade.use();
  }*/

  void Graphics::render(GLFWwindow* window,f32 dt) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    debugAssert(cam != NULL,
		"You need to give Graphics"
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    for (Node* node : nodes) {
      node->render(c, model);
    }

    view_proj = Mat4f::scale(Vec3f(1.0f/window_size.x(),
				   1.0f/window_size.y(),
				   1));
    Shader::UNI_VIEW_PROJ.registerVal(view_proj);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    for (GUINode* node : nodes_2d) {
      node->render(c, model);
    }
    
  }
}
