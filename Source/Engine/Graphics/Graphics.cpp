#include "Graphics.hpp"
#include "Input.hpp"
#include "Engine.hpp"
#include "RenderableShape.hpp"

NAMESPACE {

  Graphics::Graphics(Engine* _engine)
    : engine(_engine),
    window(NULL),
    flags(GRAPHICS_NO_FLAGS) {}

  void Graphics::init(GLFWwindow* _window) {
    
    window = _window;
    glfwGetWindowSize(window, &win_size.x(), &win_size.y());
    renderer.init(win_size);
    cam.setAspect(win_size);

    RenderableShape::init();
    
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifndef N_DEBUG
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Input::addWindowResizeCallback
      ([this](GLFWwindow* win, i32 x, i32 y) {
	if (win == window) {
	  win_size = Vec2i(x,y);
	  renderer.onWindowResize(win_size);
	  cam.setAspect(win_size);
	}
      });
  }

  void Graphics::renderFunc(ComponentPair<RenderableComp> obj,
			    RenderContext c,
			    Mat4f model) {
    Vec3f dist = cam.getTrans() - obj.obj->getTrans();
    c.dist = dist.norm();
    Mat4f comb = model*obj.obj->getMat();
    Shader::UNI_MODEL.registerVal(comb);
    obj.comp->render(c);

    if (flags & GRAPHICS_RENDER_BOUNDING_TIGHT) {
      BoundingObject* bound =
	obj.obj->getTightBoundingObject()->transform
	(obj.obj->getBasicTransform());
      bound->render(c);
      delete bound;
    }

    if (flags & GRAPHICS_RENDER_BOUNDING_LOOSE) {
      BoundingObject* bound =
	obj.obj->getLooseBoundingObject()->transform
	(obj.obj->getBasicTransform());
      bound->render(c);
      delete bound;
    }
    
  }
  
  void Graphics::render() {

    renderer.prepare();

    Shader::UNI_DIR_LIGHTS
      .registerArray(dir_lights.begin(),
		     Shader::MAX_DIR_LIGHTS);
    Shader::UNI_AMBIENT.registerVal(ambient);

    Mat4f view_proj = cam.getProj()*cam.getView();
    Shader::UNI_VIEW_PROJ.registerVal(view_proj);
    Mat4f model;
    RenderContext c;
    c.dt = engine->dt;
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    engine->traverseStatic<RenderableComp>
      (NULL, [this, c, model](ComponentPair<RenderableComp> obj) {
	renderFunc(obj, c, model);
      });
    engine->traverseDynamic<RenderableComp>
      (NULL, [this, c, model](ComponentPair<RenderableComp> obj) {
	renderFunc(obj, c, model);
      });
    
    renderer.finalize();

    view_proj = Mat4f::scale(Vec3f(1.0f/win_size.x(),
				   1.0f/win_size.y(),
				   1));
    Shader::UNI_VIEW_PROJ.registerVal(view_proj);
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    for (GUINode node : gui_nodes) {
      node.render(c, model);
    }

    glfwSwapBuffers(window);
    
  }
}
