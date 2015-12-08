#pragma once

#include "Physics.hpp"
#include "Graphics.hpp"
#include "MeshLoader.hpp"

NAMESPACE {

  struct MonkeyHead : PhysicalObject {

    MonkeyHead(Vec3f pos,
	       Vec3f veloc,
	       Physics* phys,
	       Graphics* graphics);
    virtual ~MonkeyHead();
    static StaticMesh* mesh;

  };

}
