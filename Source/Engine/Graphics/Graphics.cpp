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
    cam = engine->emplaceDynamic<Camera>();
    cam->setAspect(win_size);

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
	  cam->setAspect(win_size);
	}
      });
  }

  static inline bool renderFunc(Graphics* graphics,
				Pointer<GameObject>& obj,
				RenderableComp* comp) {
    
    BoundingObject* bound = obj->getLooseBoundingObject();
    if (!graphics->frustum.intersects(bound)) {
      return false;
    }

    Vec3f dist = graphics->cam->getTrans() - obj->getTrans();
    Mat4f model = obj->getMat();
    graphics->_addRenderable(comp, model, dist);
    
    if (graphics->flags & GRAPHICS_RENDER_BOUNDING_LOOSE) {
      RenderableComp* p = bound->getRenderable(&model);
      if (p) {
	graphics->_addRenderable(p, model, dist);
      }
    }
    if (graphics->flags & GRAPHICS_RENDER_BOUNDING_TIGHT) {
      bound = obj->getTightBoundingObject();
      RenderableComp* p = bound->getRenderable(&model);
      if (p) {
	graphics->_addRenderable(p, model, dist);
      }
    }
    return true;
  }
  
  void Graphics::_addRenderable(RenderableComp* c,
				Mat4f model,
				Vec3f dist) {
    _renderables[c].dist = dist;
    if (_renderables[c].mats.back().size() >=
	       Shader::MAX_MODEL_MATS) {
      _renderables[c].mats.emplace_back();
    }
    _renderables[c].mats.back().push_back(model);
  }

  void Graphics::initRender() {
    renderer.prepare();
    _renderables.clear();

    Shader::setFlags(SHADER_ALL_FLAGS);
    Shader::UNI_DIR_LIGHTS
      .registerArray(dir_lights.begin(),
		     Shader::MAX_DIR_LIGHTS);
    Shader::UNI_AMBIENT.registerVal(ambient);

    Mat4f view_proj = cam->getProj()*cam->getView();
    frustum = BoundingFrustum(view_proj);
    Shader::UNI_VIEW_PROJ.registerVal(view_proj);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
  }
  
  void Graphics::renderDynamic() {
    
    engine->traverseDynamic<RenderableComp>
      (&frustum, [this]
       (Pointer<DynamicObject>& obj, Pointer<RenderableComp>& comp) -> bool {
	if (renderFunc(this, (Pointer<GameObject>&)obj, comp)) {
	}
	return true;
      });
    
  }
  
  void Graphics::renderStatic() {
    
    engine->traverseStatic<RenderableComp>
      (&frustum, [this]
       (Pointer<StaticObject>& obj, Pointer<RenderableComp>& comp) -> bool {
	if (renderFunc(this, (Pointer<GameObject>&)obj, comp)) {
	}
	return true;
      });
    
  }
  
  void Graphics::finalizeRender() {

    Mat4f model;
    RenderContext c;
    c.dt = engine->dt;

    for (auto& pair : _renderables) {
      RenderableComp* rend = pair.first;
      c.dist = pair.second.dist;
      for (auto& arr : pair.second.mats) {
	c.instances = arr.size();
	PEACE_GL_CHECK_ERROR;
	Shader::setFlags(rend->shader_flags);
	PEACE_GL_CHECK_ERROR;
	Shader::UNI_MODEL.registerArray(arr);
	PEACE_GL_CHECK_ERROR;
	rend->render(c);
	PEACE_GL_CHECK_ERROR;
      }
    }
    
    renderer.finalize();

    Mat4f view_proj = Mat4f::scale(Vec3f(1.0f/win_size.x(),
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
