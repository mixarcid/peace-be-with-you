#pragma once

#include "Macros.hpp"

NAMESPACE {

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

    inline ~HandledArray() {
      for (HandledArrayElem<T> elem : arr) {
	delete elem.handle;
      }
    }

    inline T operator[](u32 index) {
      return arr[index].elem;
    }
    
    inline Iterator begin() {
      return Iterator(arr.begin());
    }
    
    inline Iterator end() {
      return Iterator(arr.end());
    }

    inline u32 size() {
      return arr.size();
    }
    
    inline ArrayHandle push_back(T inserted) {
      HandledArrayElem<T> elem;
      ArrayHandle ret = new u32(arr.size());
      elem.elem = inserted;
      elem.handle = ret;
      arr.push_back(elem);
      return ret;
    }
    
    inline void removeAndReplace(ArrayHandle h) {
      debugAssert(*h < arr.size(),
		  "You must provide a valid ArrayHandle to HandledArray's"
		  "removeAndReplace method");
      HandledArrayElem<T> back = arr.back();
      *back.handle = *h;
      arr[*h] = back;
      arr.pop_back();
      delete h;
    }
  };

}
