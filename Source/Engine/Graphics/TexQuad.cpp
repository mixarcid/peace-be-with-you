#include "TexQuad.hpp"

NAMESPACE {

  TexQuad::TexQuad(Texture texture, Vec2f halves)
    : RenderableReg(SHADER_FLAGS), tex(texture),
    b_sphere(Vec3f(0,0,0), halves.norm()){

    Array<TexQuadData> data =
      {{Vec3f(halves.x(), halves.y(), 0), Vec2f(1,1)},
       {Vec3f(halves.x(), -halves.y(), 0), Vec2f(1,0)},
       {Vec3f(-halves.x(), halves.y(), 0), Vec2f(0,1)},
       {Vec3f(-halves.x(), -halves.y(), 0), Vec2f(0,0)}};
    Array<u32> indexes = {0,2,1,1,2,3};

    RenderableReg::init();
    RenderableReg::ebo.bindArray(indexes, false);
    RenderableReg::vbo.bindArray(data, false);
    RenderableReg::vao.registerVars({Shader::POSITION, Shader::TEX_COORD});
    
  }

  void TexQuad::render(RenderContext c) {
    Shader::UNI_TEXTURE.registerTexture(tex);
    RenderableReg::render(c);
  }

}
