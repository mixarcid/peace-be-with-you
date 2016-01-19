#pragma once

#include "Containers.hpp"

NAMESPACE {

  template <typename T>
    struct Pointer {

    T* data;

    Pointer(T* _data) : data(_data) {
      data->pointers.push_back(this);
    }

    T& operator*() {
      return *data;
    }

    T* operator->() {
      return data;
    }
    
  };

  struct Pointable {

    Array<Pointer<Pointable>*> pointers;

    Pointable() {}

    Pointable(const Pointable& p) {
      for (Pointer<Pointable>* pointer : p.pointers) {
	*p = this;
	pointers.push_back(p);
      }
      p->pointers.clear();
    }

    ~Pointable() {
      for (Pointer<Pointable>* pointer : pointers) {
	*p = NULL;
      }
    }
    
  };

}
