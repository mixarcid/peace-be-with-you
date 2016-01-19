#pragma once

#include "GameObject.hpp"
#include "MeshLoader.hpp"

NAMESPACE {

  struct MonkeyHead : GameObject {

    MonkeyHead(Vec3f pos,
	       Vec3f veloc,
	       Engine* engine)
      : GameObject(mesh, Material(1, 0.5), pos, veloc) {

      $rttiConstruct("MonkeyHead");
      GameObject::init(engine);
    }
    
    static StaticMesh* mesh;

  };
  $registerRttiStruct();
}
