#pragma once

#include "GameObject.hpp"
#include "MeshLoader.hpp"
#include "PhysicsComp.hpp"


NAMESPACE {

  struct MonkeyHead : DynamicObject {

    MonkeyHead(Engine* engine, Vec3f pos, Vec3f veloc)
      : DynamicObject(engine, pos) {
      $rttiConstruct("MonkeyHead");
      init(veloc);
    }
    void init(Vec3f veloc);
    ~MonkeyHead();

    static Asset<StaticMesh> mesh;
    static Material material;
    
  };
  $registerRttiStruct();

}
