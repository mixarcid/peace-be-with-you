#pragma once

#include "Containers.hpp"

NAMESPACE {

  template <typename T>
    struct Pointer;

  struct Pointable {

    Array<Pointer<Pointable>*> pointers;

    Pointable() {}
    Pointable(Pointable& p);
    Pointable(Pointable&& p);
    
    virtual ~Pointable();
    
    void onMove();
    
    void operator=(Pointable& p);
    void operator=(Pointable&& p);

  };

  template <typename T>
    struct Pointer {

    T* data;
    Array<Pointer<Pointable>*>::Iterator it;

    Pointer(T* _data)
      : data(_data),
	it(data ? data->pointers.end() : NULL) {
      if (data) {
	data->pointers.push_back((Pointer<Pointable>*)this);
      }
    }
  
    ~Pointer() {
      if (*this) {
	data->pointers.removeAndReplace(it);
	if (data->pointers.size() > 0) {
	  (*it)->it = it;
	}
      }
    }

    void operator==(T* _data) {
      return data == _data;
    }

    bool operator!=(T* _data) {
      return data != _data;
    }

    operator bool() {
      return data;
    }
  
  
    T& operator*() {
      debugAssert(data,
		  "You have a NULL pointer");
      return *data;
    }

    T* operator->() {
      debugAssert(data,
		  "You have a NULL pointer");
      return data;
    }
    
  };

}
