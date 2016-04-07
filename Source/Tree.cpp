#include "Tree.hpp"
#include "PhysicsComp.hpp"

NAMESPACE {

  const static Material MATERIAL(800, 0.2, 0.5, 0.4);
  const u8 Tree::NUM_MODELS;
  Asset<StaticMesh> Tree::meshes[TREE_LAST] = {
    Asset<StaticMesh>("TestTree:0")
  };

  void Tree::init(TreeType type, u8 num) {
    
    StaticMesh* mesh = meshes[type*NUM_MODELS + num].get();
    Pointer<StaticObject> obj(this);
    
    tight_object.set(mesh->getTightBoundingObject());
    loose_object.set(mesh->getLooseBoundingObject());
    getTightBoundingObject()->transform(getTransform());
    getLooseBoundingObject()->transform(getTransform());
    
    addComponent(mesh);
    addComponent(new StaticPhysicsComp(obj, MATERIAL));
    
  }
  
  Tree::~Tree() {
    delete getComponent<DynamicPhysicsComp>();
  }

}
