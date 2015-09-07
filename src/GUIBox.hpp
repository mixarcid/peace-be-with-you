#pragma once

#include "GUI.hpp"

NAMESPACE {

  struct GUIBox: GUIElem {
    GUIBox(Vec2s coord,
	   Vec2s size,
	   //Size of array MUST be 4
	   Array<Vec2s> tex_coords,
	   Texture* tex,
	   u8 z_val);
  };

}
