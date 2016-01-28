#pragma once

#include "GameObject.hpp"
#include "MeshLoader.hpp"


NAMESPACE {

  struct Player : GameObject {
    
    Player(Engine* engine, Vec3f pos)
      : GameObject(engine, pos) {
      $rttiConstruct("Player");
      ptr = this;
      init();
    }
    void init();
    ~Player();

    static Pointer<Player> ptr;
    static f32 cam_speed;
    static f32 cam_rot_speed;
    static Asset<BonedMeshBase> mesh;

  };
  $registerRttiStruct();
}

