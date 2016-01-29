#include "Graphics.hpp"
#include "Input.hpp"
#include "Engine.hpp"

NAMESPACE {

  Vec2f Graphics::window_size;

  Graphics::Graphics(Engine* _engine)
    : engine(_engine) {}

  Graphics::~Graphics() {}

  void Graphics::init(const String vert, const String frag) {
    
    shade.init(vert, frag);
    shade.use();
    
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifndef N_DEBUG
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    i32 w, h;
    Input::getWindowSize(&w, &h);
    window_size = Vec2f(w,h);
    Input::addWindowResizeCallback
      ([this]
       (GLFWwindow* win,
	i32 w, i32 h) {
	cam.onWindowResize(w,h);
	window_size = Vec2f(w,h);
      });
    cam.onWindowResize(window_size.x(),
		       window_size.y());
  }
  
  void Graphics::render(GLFWwindow* window) {

    glClearColor(background_color.x(),
		 background_color.y(),
		 background_color.z(),
		 background_color.w());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader::UNI_DIR_LIGHTS
      .registerArray(dir_lights.begin(),
		     Shader::MAX_DIR_LIGHTS);
    Shader::UNI_AMBIENT.registerVal(ambient);

    Mat4f view_proj = cam.getProj()*cam.getView();
    Shader::UNI_VIEW_PROJ.registerVal(view_proj);
    Mat4f model;
    RenderContext c(engine->dt);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    for (GameObject obj : engine->game_objects) {
      Pointer<RenderableComp> rend = obj.getComponent<RenderableComp>();
      if (rend) {
	Mat4f comb = model*obj.transform.getMat();
	Shader::UNI_MODEL.registerVal(comb);
	rend->render(c);
      }
    }

    /*view_proj = Mat4f::scale(Vec3f(1.0f/window_size.x(),
				   1.0f/window_size.y(),
				   1));
    Shader::UNI_VIEW_PROJ.registerVal(view_proj);
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    for (GUINode node : gui_nodes) {
      node.render(c, model);
      }*/

    glfwSwapBuffers(window);
    
  }
}
