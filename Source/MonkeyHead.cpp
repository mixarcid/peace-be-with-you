#include "MonkeyHead.hpp"

NAMESPACE {

  Asset<StaticMesh> MonkeyHead::mesh("Monkey:Suzanne");
  Material MonkeyHead::material(5, 0.5, 0.2, 0.1);
  
  void MonkeyHead::init(Vec3f veloc) {
    tight_object.set(mesh.get()->getTightBoundingObject());
    loose_object.set(mesh.get()->getLooseBoundingObject());
    addComponent(mesh.get());
    addComponent(new PhysicsComp(this, material, veloc));
  }

  MonkeyHead::~MonkeyHead() {
    delete getComponent<PhysicsComp>();
  }

}
