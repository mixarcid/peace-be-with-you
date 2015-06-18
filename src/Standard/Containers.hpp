#pragma once

#include <vector>
#include <unordered_map>

NAMESPACE {

  template <typename T>
    using Array = typename std::vector<T>;

  template <typename T>
    void removeAndReplace(Array<T> arr, unsigned int index) {
    arr[index] = arr.back();
    arr.pop_back();
  }
  
  template <typename Key, typename Val>
    using HashMap = typename std::unordered_map<Key, Val>;
}

