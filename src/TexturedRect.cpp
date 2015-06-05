#include "TexturedRect.h"
#include "Shader.h"

NAMESPACE {

  unsigned int TexturedRect::vao;
  
  static bool firstVAO = true;
  
  TexturedRect::TexturedRect(const char* filename) {
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
    
    if (firstVAO) {
      TexturedRect::vao = VAO::getNextVAOIndex();
      firstVAO = false;
    }
    
    VAO::all_vaos.use(TexturedRect::vao);
    vbo.bindArray(data, false);
    ebo.bindArray(elems, false);
    tex.use();
    tex.load(filename, Shader::UNI_TEXTURE);
    VAO::all_vaos.registerVars({Shader::POSITION,
    	  Shader::TEX_COORD}, TexturedRect::vao);
  }

  void TexturedRect::render() {
    VAO::all_vaos.use(TexturedRect::vao);
    tex.use();
    ebo.draw(GL_TRIANGLES);
  }
}
