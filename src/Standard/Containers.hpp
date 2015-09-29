#pragma once

#include <vector>
#include <unordered_map>
#include <initializer_list>
#include "Types.hpp"
#include "Function.hpp"

NAMESPACE {

  template <typename T>
    using Array = typename std::vector<T>;
  template <typename Key, typename Val>
    using HashMap = typename std::unordered_map<Key, Val>;
  template <typename A, typename B>
    using Pair = typename std::pair<A, B>;
  template <typename T>
    using InitList = typename std::initializer_list<T>;
}

