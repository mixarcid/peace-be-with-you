#pragma once

#include "Message.hpp"
#include "Function.hpp"
#include "Pointable.hpp"

NAMESPACE {

  //to be send to objects when they are spacially of the bounds
  //of the QuadTree, Grid, or other spacial container.
  //when the object is messaged, it changes should_insert to true
  //if it wants to be inserted regardless
  struct OutOfBoundsMessage : Message {
    bool should_insert;
    OutOfBoundsMessage() : should_insert(false) {}
  };

  struct GameObject;

  typedef function<bool(Pointer<GameObject>&)> ContainerCallback;

}