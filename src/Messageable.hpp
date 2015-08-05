#pragma once

#include "Standard.hpp"
#include "RTTI.hpp"

NAMESPACE {

  struct Message {};

  struct Messageable {
    virtual void message(Message m) {}
  };
  
}
