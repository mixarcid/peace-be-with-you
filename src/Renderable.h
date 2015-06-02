#pragma once

#include "Standard.h"

NAMESPACE {

  struct Renderable {
    virtual void render() = 0;
    //virtual ~Renderable() = 0;
  };
  
}
