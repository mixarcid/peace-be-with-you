#pragma once

#include "Standard.hpp"

NAMESPACE {

  struct Message {};

  struct Messageable {
    virtual void message(Message m) = 0;
  };
  
}
