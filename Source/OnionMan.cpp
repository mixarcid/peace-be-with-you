#include "OnionMan.hpp"
#include "Engine.hpp"
#include "Player.hpp"

NAMESPACE {

  Asset<StaticMesh> OnionMan::mesh("OnionMan:Man");
  Material OnionMan::material(20, 0.9, 0.05, 0.02);
  
  void OnionMan::init(Vec3f veloc) {
    tight_object.set(mesh.get()->getTightBoundingObject());
    loose_object.set(mesh.get()->getLooseBoundingObject());
    getTightBoundingObject()->transform(getTransform());
    getLooseBoundingObject()->transform(getTransform());
    addComponent(mesh.get());
    Pointer<DynamicObject> obj(this);
    addComponent(new DynamicPhysicsComp(obj, material, veloc));
  }

  OnionMan::~OnionMan() {
    delete getComponent<DynamicPhysicsComp>();
  }

  void OnionMan::release() {

    for (f32 theta = 0; theta < degreesToRadians(360.0); theta += degreesToRadians(10.0)) {
      
      Quaternionf q(0,0,degreesToRadians(90.0) + theta);
      Vec3f diff(cos(theta)*100, sin(theta)*100, 100);
      auto onion = Engine::emplaceDynamic<OnionMan>(Player::ptr->getTrans() + diff, Vec3f(0,0,0));
      onion->rotAbs(q);
      
    }
    
  }

}
