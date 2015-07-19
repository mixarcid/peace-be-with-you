#pragma once

#include <vector>
#include <unordered_map>
#include "Types.hpp"

NAMESPACE {

  template <typename T>
    using Array = typename std::vector<T>;

  template <typename T>
    void removeAndReplace(Array<T> arr, u32 index) {
    arr[index] = arr.back();
    arr.pop_back();
  }
  
  template <typename Key, typename Val>
    using HashMap = typename std::unordered_map<Key, Val>;
  template <typename A, typename B>
    using Pair = typename std::pair<A, B>;
}

