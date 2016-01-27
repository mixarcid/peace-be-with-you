#pragma once

#include "GameObject.hpp"
#include "MeshLoader.hpp"


NAMESPACE {

  struct Player : GameObject {
    
    Player(Engine* engine, Vec3f pos)
      : GameObject(engine, pos) {
      $rttiConstruct("Player");
      init();
    }
    
    void init();
    
    static f32 cam_speed;
    static f32 cam_rot_speed;
    static StaticMesh* mesh;

  };
  $registerRttiStruct();
}

