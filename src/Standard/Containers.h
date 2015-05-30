#pragma once

#include <string>
#include <vector>

NAMESPACE {
  
  typedef std::string String;
  using std::to_string;

  template <typename T>
    using Array = typename std::vector<T>;
}
