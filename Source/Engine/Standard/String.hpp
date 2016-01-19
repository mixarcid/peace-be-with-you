#pragma once

#include <string>
#include "Macros.hpp"

NAMESPACE {

  typedef std::string String;
  using std::to_string;

  namespace str {
    //Thanks, Larry Gritz from Stack Overflow
    String vformat(const char* fmt, va_list ap);
    String format(const char* fmt, ...);
  }

  template <typename T>
    String to_string(T* p) {
    return str::format("%p", p);
  }
}
