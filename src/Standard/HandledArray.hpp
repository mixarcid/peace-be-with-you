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
	++it;
      }
      T operator*() const {
	return (*it).elem;
      }
      bool operator==(Iterator b) const {
	return it == b.it;
      }
      bool operator!=(Iterator b) const {
	return it != b.it;
      }
    };

    Array<HandledArrayElem<T>> arr;

    inline ~HandledArray() {
      for (HandledArrayElem<T> elem : arr) {
	//printf("handle: %p\n", elem.handle);
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

    /*
      crude implimentation, but it ain't bad for
      small arrays

      cmp returns true if we want the second element to
      go behind the first, false otherwise
    */
    ArrayHandle insertSorted(T item,
			     function<bool(T,T)> cmp) {
      HandledArrayElem<T> elem;
      elem.elem = item;
      
      if (arr.size() > 0) {
	
	Iterator it(arr.begin());
	u32 index;
	
	for (; it != end(); ++it, ++index) {
	  if (cmp(*it, item)) {
	    elem.handle = new u32(index);
	    it = arr.insert(it.it, elem);
	    break;
	  }
	}
	
	if (it == end()) { //we haven't inserted it
	  elem.handle = new u32(arr.size());
	  arr.push_back(elem);
	} else {
	  //re-assign all subsequent handles
	  while (it != end()) {
	    ++(*((*it.it).handle));
	    ++it;
	  }
	}
	
      } else {
	elem.handle = new u32(0);
	arr.push_back(elem);
      }
      return elem.handle;
    }
    
    inline void removeAndReplace(ArrayHandle h) {
      if (h) {
	debugAssert(*h < arr.size(),
		    "You must provide a valid ArrayHandle to HandledArray's"
		    "removeAndReplace method");
	HandledArrayElem<T> back = arr.back();
	*back.handle = *h;
	arr[*h] = back;
	arr.pop_back();
	delete h;
      }
    }
  };

}
