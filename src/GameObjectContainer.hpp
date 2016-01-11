#pragma once

#include "Quadtree.hpp"

NAMESPACE {

  //Dynamic objects stored in easily manageable "sectors"
  typedef HashMap<Vec2u, HandledArray<GameObject*>> DynamicObjectContainer;

  struct GameObjectHandle {
    GameObjectFlags flags;
    Vec2u sector;
    ArrayHandle dynamic_handle;
    ArrayHandle update_handle;
    String object_name;
  };

  struct GameObjectContainer {

    Vec3f center;
    Quadtree static_objects;
    DynamicObjectContainer dynamic_objects;
    HandledArray<GameObject*> update_objects;
    HashMap<String, GameObject*> named_objects;

    GameObjectHandle addGameObject(GameObject* obj);
    void removeGameObject(GameObjectHandle h);

    void traverseStaticObjects(function<bool(BoundingAABB)> test,
			       function<void(GameObject*)> callback);
    void traverseDynamicSectors(function<bool(BoundingAABB)> test,
				function<void(HandledArray<GameObject*>)>);
  };

}
