#pragma once

#include "Containers.hpp"

NAMESPACE {

  template <typename T>
    struct Pointer;

  struct Pointable {

    Array<Pointer<Pointable>*> pointers;

    Pointable() {}
    Pointable(const Pointable& p);
    Pointable(Pointable&& p);
    
    virtual ~Pointable();
    
    void operator=(const Pointable& p);
    void operator=(Pointable&& p);

  };

  template<>
     void onMove(Pointable* ptr);

  template <typename T>
    struct Pointer {

    Pointable* data;
    Array<Pointer<Pointable>*>::Iterator it;

    Pointer(Pointable* _data = NULL)
      : data(_data),
	it(NULL) {
      if (data) {
	it = data->pointers.push_back((Pointer<Pointable>*)this);
      }
    }

    Pointer(const Pointer& ptr)
      : data(ptr.data),
	it(NULL) {
      if (data) {
	it = data->pointers.push_back((Pointer<Pointable>*)this);
      }
    }

    Pointer(Pointer&& ptr)
      : data(ptr.data),
	it(ptr.it) {
      if (data) {
	*it = (Pointer<Pointable>*) this;
      }
      ptr.data = NULL;
    }

    template <typename U>
    explicit Pointer(const Pointer<U>& ptr)
      : data((T*) ptr.data),
	it(NULL) {
      if (data) {
	it = data->pointers.push_back((Pointer<Pointable>*)this);
      }
    }

    template <typename U>
    explicit Pointer(Pointer<U>&& ptr)
      : data((T*) ptr.data),
	it(ptr.it) {
      if (data) {
	*it = (Pointer<Pointable>*) this;
      }
      ptr.data = NULL;
    }
  
    ~Pointer() {
      if (*this) {
	auto p1 = data;
	auto p2 = &(p1->pointers);
	p2->removeAndReplace(it);
      }
    }
    
    Pointer& operator=(const Pointer& ptr) {
      if (*this) {
	(data)->pointers.removeAndReplace(it);
      }
      data = ptr.data;
      if (data) {
	it = data->pointers.push_back((Pointer<Pointable>*)this);
      } else {
	it = NULL;
      }
      return *this;
    }

    Pointer& operator=(Pointer&& ptr) {
      if (*this) {
	data->pointers.removeAndReplace(it);
      }
      data = ptr.data;
      if (data) {
	it = ptr.it;
        *it = (Pointer<Pointable>*) this;
	ptr.data = NULL;
      }
      return *this;
    }

    void operator==(Pointable* _data) const {
      return data == _data;
    }

    bool operator!=(Pointable* _data) const {
      return data != _data;
    }

    operator bool() const {
      return data;
    }

    operator T*() const {
      return (T*) data;
    }
  
    T& operator*() const {
      debugAssert(data,
		  "You are trying to dereference a NULL pointer");
      return (T&) *data;
    }

    T* operator->() const {
      debugAssert(data,
		  "You are trying to dereference a NULL pointer");
      return (T*) data;
    }
    
  };

  template<>
    void onMove(Pointer<Pointable>** ptr);

}
