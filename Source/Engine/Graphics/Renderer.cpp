#include "Renderer.hpp"

NAMESPACE {

  Renderer::Renderer() : second_shade(SHADER_PLAIN) {}
  
  void Renderer::init(Vec2i win_size) {
    PEACE_GL_CHECK_ERROR;
    first_shade.init("Main", "Main");
    PEACE_GL_CHECK_ERROR;
    first_shade.bindOutputs({"outColor","outNormal"});
    PEACE_GL_CHECK_ERROR;
    second_shade.init("PassThrough", "Toon");
    PEACE_GL_CHECK_ERROR;
    second_shade.bindOutputs({"outColor"});
    PEACE_GL_CHECK_ERROR;
    first_shade.use();
    PEACE_GL_CHECK_ERROR;
    
    screen_coord = second_shade.getVar("screenCoord", SHADER_TYPE_VECTOR2F);
    diffuse_uniform = second_shade.getUniform("diffuse", 0);
    normal_uniform = second_shade.getUniform("normal", 1);
    screen_quad.init(screen_coord);

    onWindowResize(win_size);
  }

  void Renderer::onWindowResize(Vec2i win_size) {
    window_size = win_size;
    
    g_buffer.init();
    g_buffer.addDepthBuffer(win_size);
    
    diffuse.init();
    diffuse.createEmpty(win_size);
    normal.init();
    normal.createEmpty(win_size);
    
    Array<Texture> textures;
    textures.push_back(diffuse);
    textures.push_back(normal);
    g_buffer.bindTargets(textures);
    
    debugAssert(g_buffer.isComplete(),
		"The G Buffer is not complete");
  }
  
  void Renderer::prepare() {
    first_shade.use();
    g_buffer.clearTargets({Vec4f(1,1,1,1), Vec4f(0.5,0.5,-0.5,0)});
    g_buffer.use();
    glViewport(0,0,window_size.x(),window_size.y());
  }
  
  void Renderer::finalize() {
    GLenum poly_mode = gl::getPolygonMode();
    gl::setPolygonMode(GL_FILL);
    second_shade.use();
    FBO::useScreen();
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    diffuse_uniform.registerTexture(diffuse);
    normal_uniform.registerTexture(normal);
    screen_quad.render();
    gl::setPolygonMode(poly_mode);
  }

}
