#pragma once

#include <exception>
#include "Macros.h"
#include "String.h"

NAMESPACE {

  typedef std::exception Exception;

  struct FatalError : Exception {

    String msg;
    
    FatalError(const String message) {
      msg = message;
    }

    virtual const char* what() const throw () {
      return msg.c_str();
    }
  };
  
}
