#include "GameObject.hpp"
#include "Engine.hpp"

NAMESPACE {

  GameObject::GameObject(Renderable* rend,
			 Material mat,
			 Vec3f xi,
			 Vec3f vi,
			 PhysicalObjectFlags physics_flags,
			 GameObjectFlags flags)
    : PhysicalObject(rend,
		     mat,
		     xi,
		     vi,
		     physics_flags),
    object_flags(flags) {}
  
  void GameObject::init(Engine* engine) {
    this->engine = engine;
    this->handle = engine->addGameObject(this);
  };

  GameObject::~GameObject() {
    engine->removeGameObject(handle);
  }

  NamedObject::NamedObject(String _name,
			   Renderable* rend,
			   Material mat,
			   Vec3f xi,
			   Vec3f vi,
			   PhysicalObjectFlags physics_flags,
			   GameObjectFlags flags)
    : GameObject(rend,
		 mat,
		 xi,
		 vi,
		 physics_flags,
		 flags | OBJECT_NAMED),
    name(_name) {}
  
}
