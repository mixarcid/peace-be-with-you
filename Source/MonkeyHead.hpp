#pragma once

#include "GameObject.hpp"
#include "MeshLoader.hpp"
#include "PhysicsComp.hpp"


NAMESPACE {

  struct MonkeyHead : DynamicObject {

    MonkeyHead(Vec3f pos, Vec3f veloc)
      : DynamicObject(pos) {
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
