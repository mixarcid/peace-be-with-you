#include "Water.hpp"
#include "Engine.hpp"

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

  void Water::init(Vec3f pos, Vec2f halves) {
    addComponent(new WaterRenderable(halves));
    BoundingAABB aabb(pos, Vec3f(halves, 10));
    GameObject::loose_object.set(&aabb);
    GameObject::tight_object.set(&aabb);
    Pointer<DynamicObject> t(this);
    Engine::engine->graphics.cam->addChild(t, Transform(Vec3f(0,0,0)), CHILD_OBJECT_TRANSLATE_NO_Z);
  }
  
  void Water::message(Message* msg) {
    switch(typeId(msg)) {
    case typeId<OutOfBoundsMessage>():
      ((OutOfBoundsMessage*)msg)->should_insert = true;
      break;
    }
  }

}
