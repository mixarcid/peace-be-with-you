#pragma once

#include "Quadtree.hpp"

NAMESPACE {

  struct GameObjectHandle {
    ArrayHandle handle;
  };

  struct GameObjectContainer {

    Vec3f center;
    Quadtree static_objects;
    DynamicObjectContainer dynamic_objects;

    GameObjectHandle addGameObject(GameObject* obj);
    void removeGameObject(GameObjectHandle h);

    void traverseStaticObjects(function<bool(BoundingAABB)> test,
			       function<void(GameObject*)> callback);
    void traverseDynamicObjects(function<bool(BoundingAABB)> test,
				function<void(GameObject*)>);
  };

}
