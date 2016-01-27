#pragma once

#include "RTTI.hpp"
#include "Pointable.hpp"

NAMESPACE {

  struct GameObject;
  
  struct Component : BaseRTTI, Pointable {

    Component() {}
    virtual ~Component() {}
    
  };
  
}
