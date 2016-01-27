#pragma once

#include "RTTI.hpp"

NAMESPACE {

  struct Message : BaseRTTI {};

  struct Messageable {
    virtual void message(Message* m) {}
  };
  
}
