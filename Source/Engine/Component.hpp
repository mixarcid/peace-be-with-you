#pragma once

#include "RTTI.hpp"
#include "Pointable.hpp"

NAMESPACE {

  struct GameObject;
  
  struct Component : Pointable {

    Component() {
      $rttiConstruct("Component");
    }
    virtual ~Component() {}
    
  };
  $registerRttiStruct();
  
}
