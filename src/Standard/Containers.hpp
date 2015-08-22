#pragma once

#include <vector>
#include <unordered_map>
#include <initializer_list>
#include "Types.hpp"

NAMESPACE {

  template <typename T>
    using Array = typename std::vector<T>;
  
  template <typename Key, typename Val>
    using HashMap = typename std::unordered_map<Key, Val>;
  template <typename A, typename B>
    using Pair = typename std::pair<A, B>;
  template <typename T>
    using InitList = typename std::initializer_list<T>;

  //HandedArray stuff
  typedef u32* ArrayHandle;
  
  template <typename T>
    struct HandledArrayElem {
    T elem;
    ArrayHandle handle;
  };

  template <typename T>
    struct HandledArray {

    struct Iterator {
      typename Array<HandledArrayElem<T>>::iterator it;
      Iterator(typename
	       Array<HandledArrayElem<T>>::iterator _it)
	: it(_it) {}
      void operator++() {
	it++;
      }
      T operator*() const {
	return (*it).elem;
      }
      bool operator!=(Iterator b) const {
	return it != b.it;
      }
    };

    Array<HandledArrayElem<T>> arr;

    Iterator begin() {
      return Iterator(arr.begin());
    }
    
    Iterator end() {
      return Iterator(arr.end());
    }
    
    ArrayHandle push_back(T inserted) {
      HandledArrayElem<T> elem;
      ArrayHandle ret = new u32(arr.size());
      elem.elem = inserted;
      elem.handle = ret;
      arr.push_back(elem);
      return ret;
    }
    
    void removeAndReplace(ArrayHandle h) {
      HandledArrayElem<T> back = arr.back();
      *back.handle = *h;
      arr[*h] = back;
      arr.pop_back();
      delete h;
    }
  };
  
}

