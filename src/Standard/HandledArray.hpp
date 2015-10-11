#pragma once

#include "Containers.hpp"

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

    //crude implimentation, but it ain't bad for small arrays
    ArrayHandle insertSorted(T item,
			     function<bool(T,T)> cmp) {
      HandledArrayElem<T> elem;
      elem.elem = item;
      
      if (arr.size() > 0) {
	
	ArrayHandle ret;
	Iterator it(arr.begin());
	u32 index;
	
	for (; it != end(); ++it, ++index) {
	  if (cmp(*it, item)) {
	    ret = new u32(index);
	    elem.handle = ret;
	    arr.insert(it.it, elem);
	    break;
	  }
	}
	while (it != end()) {
	  ++*((*it.it).handle);
	  ++it;
	}
	return ret;
	
      } else {
	arr.push_back(elem);
	return new u32(0);
      }
      
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
