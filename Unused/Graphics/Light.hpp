#pragma once

#include "Vector.hpp"

NAMESPACE {

struct DirLight {
  Vec3f dir;
  f32 pad0[1];
  Vec3f color;
  
  DirLight(Vec3f _dir = Vec3f(0,0,0),
	   Vec3f _color = Vec3f(0,0,0));
};


}
