#include "Exception.hpp"
#include "Log.hpp"

NAMESPACE {

  const char* FatalError::what() const throw () {
    return msg.c_str();
  }

}
