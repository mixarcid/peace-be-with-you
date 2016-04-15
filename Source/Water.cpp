#include "Water.hpp"


NAMESPACE {

  WaterRenderable::WaterRenderable(Vec2f halves)
    : RenderableReg(SHADER_WATER | SHADER_3D) {

    Array<Vec3f> data =
      {Vec3f(halves.x(), halves.y(), 0),
       Vec3f(halves.x(), -halves.y(), 0),
       Vec3f(-halves.x(), halves.y(), 0),
       Vec3f(-halves.x(), -halves.y(), 0)};
    Array<u32> indexes = {0,2,1,1,2,3};

    RenderableReg::init();
    RenderableReg::ebo.bindArray(indexes, false);
    RenderableReg::vbo.bindArray(data, false);
    RenderableReg::vao.registerVars({Shader::POSITION});
    
  }

}
