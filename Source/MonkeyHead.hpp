#pragma once

#include "GameObject.hpp"
#include "MeshLoader.hpp"
#include "PhysicsComp.hpp"


NAMESPACE {

  struct MonkeyHead : GameObject {

    MonkeyHead(Engine* engine, Vec3f pos, Vec3f veloc)
      : GameObject(engine, pos) {
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
