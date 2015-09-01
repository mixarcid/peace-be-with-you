#pragma once

#include "Renderable2d.hpp"
#include "Matrix.hpp"

NAMESPACE {

  struct Node2d {

    Mat3f transform;
    Array<Node2d*> children;
    Array<Renderable2d*> renderables;

    void addChild(Node2d* child);
    void addRenderable(Renderable2d* renderable);
    Array<Renderable2dData> getData();
  }

}
