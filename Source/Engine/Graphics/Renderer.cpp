#include "Renderer.hpp"

NAMESPACE {

  Renderer::Renderer() : second_shade(SHADER_PLAIN) {}
  
  void Renderer::init(Vec2i win_size) {
    
    first_shade.init("First", "First");
    first_shade.bindOutputs({"outColor", "outNormal", "outDepth"});
    second_shade.init("PassThrough", "Second");
    second_shade.bindOutputs({"outColor"});
    first_shade.use();
    PEACE_GL_CHECK_ERROR;
    
    screen_coord = second_shade.getVar("screenCoord", SHADER_TYPE_VECTOR2F);
    diffuse_uniform = second_shade.getTexUniform("diffuse");
    normal_uniform = second_shade.getTexUniform("normal");
    depth_uniform = second_shade.getTexUniform("depth");
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
    depth.init();
    depth.createEmpty(win_size, GL_RED);
    
    Array<Texture> textures;
    textures.push_back(diffuse);
    textures.push_back(depth);
    textures.push_back(normal);
    g_buffer.bindTargets(textures);
    
    debugAssert(g_buffer.isComplete(),
		"The G Buffer is not complete");
  }
  
  void Renderer::prepare(Vec4f back_color) {
    first_shade.use();
    g_buffer.clearTargets({back_color,  Vec4f(1,0,0,1), Vec4f(0,0,0,1)});
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
    depth_uniform.registerTexture(depth);
    screen_quad.render();
    gl::setPolygonMode(poly_mode);
  }

}
