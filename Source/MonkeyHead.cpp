#include "MonkeyHead.hpp"
#include "Player.hpp"
#include "Physics.hpp"

NAMESPACE {

  Asset<StaticMesh> MonkeyHead::mesh("Monkey:Suzanne");
  Material MonkeyHead::material(5, 0.5, 0.2, 0.1);
  
  void MonkeyHead::init(Vec3f veloc) {
    tight_object.set(mesh.get()->getTightBoundingObject());
    loose_object.set(mesh.get()->getLooseBoundingObject());
    getTightBoundingObject()->transform(getTransform());
    getLooseBoundingObject()->transform(getTransform());
    addComponent(mesh.get());
    Pointer<DynamicObject> obj(this);
    addComponent(new DynamicPhysicsComp(obj, material, veloc));
    addComponent(new AudioComp());
  }

  MonkeyHead::~MonkeyHead() {
    delete getComponent<DynamicPhysicsComp>();
    delete getComponent<AudioComp>();
  }

  void MonkeyHead::message(Message* msg) {
    switch(typeId(msg)) {
      
      case typeId<CollisionMessage>(): {

	Pointer<GameObject>& obj = ((CollisionMessage*) msg)->object;
	if (typeId(obj) == typeId<Player>()) {
	  getComponent<AudioComp>()->play("die", this);
	}
	
	break;
      }
	
    }
  }

}
