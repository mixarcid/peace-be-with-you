#include "RenderableShape.hpp"

NAMESPACE {

  RenderableShape RenderableShape::CUBE(Vec4f(1,0,0,1));
  RenderableShape RenderableShape::SPHERE(Vec4f(0,1,0, 1));

  RenderableShapeData::RenderableShapeData(Vec3f _pos,
					   Vec4f _color)
    : pos(_pos), color(_color) {}
  
  RenderableShape::RenderableShape(Vec4f _color)
    : RenderableReg(SHADER_USE_COLOR | SHADER_3D), color(_color) {}
  
  void RenderableShape::render(RenderContext c) {
    this->vao.use();
    this->ebo.draw(GL_LINES);
  }

  void RenderableShape::init() {

    Array<RenderableShapeData> cube_data =
      {RenderableShapeData
       (Vec3f(-1.0f,-1.0f,-1.0f), CUBE.color),
       RenderableShapeData
       (Vec3f(-1.0f,-1.0f, 1.0f), CUBE.color),
       RenderableShapeData
       (Vec3f(-1.0f, 1.0f,-1.0f), CUBE.color),
       RenderableShapeData
       (Vec3f(-1.0f, 1.0f, 1.0f), CUBE.color),
       RenderableShapeData
       (Vec3f( 1.0f,-1.0f,-1.0f), CUBE.color),
       RenderableShapeData
       (Vec3f( 1.0f,-1.0f, 1.0f), CUBE.color),
       RenderableShapeData
       (Vec3f( 1.0f, 1.0f,-1.0f), CUBE.color),
       RenderableShapeData
       (Vec3f( 1.0f, 1.0f, 1.0f), CUBE.color)};
    
    Array<u32> cube_index_buffer =
      {0,1, 0,2, 0,4,
       1,3, 1,5,
       2,3, 2,6,
       3,7,
       4,5, 4,6,
       5,7,
       6,7};

    CUBE.RenderableReg::init();
    CUBE.ebo.bindArray(cube_index_buffer, false);
    CUBE.vbo.bindArray(cube_data, false);
    CUBE.vao.registerVars({Shader::POSITION, Shader::COLOR});
    
  }

}
