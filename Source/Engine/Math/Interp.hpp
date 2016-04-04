#pragma once

#include "Vector.hpp"

NAMESPACE {

  template <typename T>
    inline T biLerp(Vec2f coord, T& bl, T& br, T& tl, T& tr) {
    debugAssert(coord.x() <= 1.0f &&
		coord.y() <= 1.0f &&
		coord.x() >= 0.0f &&
		coord.y() >= 0.0f,
		"The components of coord must between 0 and 1 for biLerp");
    T avg_x[2];
    avg_x[0] = tr*(1-coord.x()) + br*(coord.x());
    avg_x[1] = tl*(1-coord.x()) + bl*(coord.x());
    return avg_x[0]*(1-coord.y()) + avg_x[1]*(coord.y());
  }

  template <typename T>
    inline T biLerp(Vec2f coord, T* data) {
    return biLerp(coord, data[0], data[1], data[2], data[3]);
  }

  template <typename T>
    inline T biCosInterp(Vec2f coord, T& bl, T& br, T& tl, T& tr) {
    Vec2f coord2((1-cos(coord.x()*M_PI))/2,
		 (1-cos(coord.y()*M_PI))/2);
    return biLerp(coord2, bl, br, tl, tr);
  }

  template <typename T>
    inline T biCosInterp(Vec2f coord, T* data) {
    return biCosInterp(coord, data[0], data[1], data[2], data[3]);
  }
  
}
