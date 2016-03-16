#pragma once

#include "GameObject.hpp"
#include "MeshLoader.hpp"


NAMESPACE {

  struct Player : DynamicObject {
    
    Player(Vec3f pos)
      : DynamicObject(pos) {
      $rttiConstruct("Player");
      ptr = this;
      init();
    }
    void init();
    ~Player();

    static ChildObject* camera;
    static Pointer<Player> ptr;
    static Transform camera_diff;
    static f32 cam_speed;
    static f32 cam_rot_speed;
    static Asset<BonedMeshBase> mesh;

  };
  $registerRttiStruct();
}

