#pragma once

#include "GameObject.hpp"
#include "MeshLoader.hpp"
#include "PhysicsComp.hpp"


NAMESPACE {

  struct OnionMan : DynamicObject {

    OnionMan(Vec3f pos, Vec3f veloc)
      : DynamicObject(pos) {
      $rttiConstruct("OnionMan");
      init(veloc);
    }
    void init(Vec3f veloc);
    ~OnionMan();

    static Asset<StaticMesh> mesh;
    static Material material;

    static void release();
  };
  $registerRttiStruct();

}
