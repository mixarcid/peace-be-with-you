#include "Sun.hpp"
#include "Engine.hpp"

NAMESPACE {

  f32 Sun::player_dist;
  Asset<Texture> Sun::texture("Sun");

  void Sun::init() {

    Vec3f trans(1,1,1);
    trans.normalize();
    Engine::engine->graphics.emplaceDirLight(-trans,
					     Vec3f(1,1,1));
    Vec3f init(0,0,1);
    Vec3f fin(trans.x(), trans.z(), trans.y());
    Vec3f c = Vec3f::cross(init, -fin);
    Quaternionf q(c.x(), c.y(), c.z(),
		  1.0f + Vec3f::dot(init, -fin));
    q.normalize();
    rotAbs(q);
    Pointer<DynamicObject> t(this);
    Engine::engine->graphics.cam->addChild(t, Transform(fin*player_dist), CHILD_OBJECT_TRANSLATE);
    
  }
  
  Sun::~Sun() {
    delete getComponent<RenderableComp>();
  }
  
}
