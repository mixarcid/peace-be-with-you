#pragma once

#include "PhysicsComp.hpp"
#include "GameObject.hpp"

NAMESPACE {

  bool resolveCollision(Pointer<DynamicObject>& a_obj,
			Pointer<DynamicPhysicsComp>& a_comp,
			Pointer<DynamicObject>& b_obj,
			Pointer<DynamicPhysicsComp>& b_comp);
  bool resolveCollision(Pointer<DynamicObject>& a_obj,
			Pointer<DynamicPhysicsComp>& a_comp,
			Pointer<StaticObject>& b_obj,
			Pointer<StaticPhysicsComp>& b_comp);
  
}
