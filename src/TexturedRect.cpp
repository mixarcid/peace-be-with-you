#include "TexturedRect.hpp"
#include "Shader.hpp"

NAMESPACE {

  TexturedRect::TexturedRect(const char* filename) {
    
    Renderable<RectData>::data = {
      {Vec3f(-0.5, 0.5, 0), Vec2f(0,0)},
      {Vec3f(0.5, 0.5, 0), Vec2f(1,0)},
      {Vec3f(0.5, -0.5, 0), Vec2f(1,1)},
      {Vec3f(-0.5, -0.5, 0), Vec2f(0,1)}
    };
    RenderableEBO<RectData>::elements = {
      0, 1, 2,
      2, 3, 0
    };
    RenderableEBO<RectData>::init({Shader::POSITION,
	  Shader::TEX_COORD}, false);

    tex = Texture::getTexture();
    tex.use();
    tex.load(filename, Shader::UNI_TEXTURE);
  }

  void TexturedRect::render() {
    tex.use();
    RenderableEBO<RectData>::render();
  }
}

/*NAMESPACE {

  unsigned int TexturedRect::vao;
  
  static bool firstVAO = true;
  
  TexturedRect::TexturedRect(const char* filename) {

    if (firstVAO) {
      TexturedRect::vao = VAO::getNextVAOIndex();
      firstVAO = false;
    }
    VAO::all_vaos.use(TexturedRect::vao);
    
    data = {
      {Vec3f(-0.5, 0.5, 0), Vec2f(0,0)},
      {Vec3f(0.5, 0.5, 0), Vec2f(1,0)},
      {Vec3f(0.5, -0.5, 0), Vec2f(1,1)},
      {Vec3f(-0.5, -0.5, 0), Vec2f(0,1)}
    };
    elems = {
      0, 1, 2,
      2, 3, 0
    };
    
    vbo.bindArray(data, false);
    ebo.bindArray(elems, false);
    VAO::all_vaos.registerVars({Shader::POSITION,
	  Shader::TEX_COORD}, TexturedRect::vao);
    tex.use();
    tex.load(filename, Shader::UNI_TEXTURE);
  }

  void TexturedRect::render() {
    VAO::all_vaos.use(TexturedRect::vao);
    tex.use();
    ebo.draw(GL_TRIANGLES);
  }
}*/
