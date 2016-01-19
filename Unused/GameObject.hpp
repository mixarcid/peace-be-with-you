#pragma once

#include "GameObjectContainer.hpp"
#include "Messageable.hpp"

NAMESPACE {

  struct Engine;
  struct Component;
  
  struct GameObject : Transform, Messageable {

    Engine* engine;
    GameObjectHandle handle;
    Array<Component*> components;

    GameObject(Engine* _engine);
    //remember to always call before regular destruction
    //if you want to remove the object before regular
    //destruction at the end, you need to call:
    //destroy();
    //engine->removeGameObject(handle);
    virtual void destroy();

    //returns NULL if it doesn't have component
    template<typename T>
    Component* getComponent();
    void addComponent(Component* component);

    //for either graphics or physics
    virtual BoundingObject getTightBoundingObject();
    virtual BoundingObject getLooseBoundingObject();
    virtual BoundingAABB getBoundingAABB();
    
  };
  
}
