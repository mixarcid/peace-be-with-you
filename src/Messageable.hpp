#pragma once

#include "Standard.hpp"
#include "RTTI.hpp"

NAMESPACE {

  struct Message : BaseRTTI {};

  struct Messageable : BaseRTTI {
    virtual void message(Message* m) {}
  };
  
}
