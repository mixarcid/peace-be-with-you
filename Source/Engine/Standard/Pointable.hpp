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
    
    void onMove();
    
    void operator=(const Pointable& p);
    void operator=(Pointable&& p);

  };

  template <typename T>
    struct Pointer {

    T* data;
    Array<Pointer<Pointable>*>::Iterator it;

    Pointer(T* _data = NULL)
      : data(_data),
	it(data ? ((Pointable*)data)->pointers.end() : NULL) {
      if (data) {
	((Pointable*)data)->pointers.push_back((Pointer<Pointable>*)this);
      }
    }

    Pointer(const Pointer& ptr)
      : data(ptr.data),
	it(data ? ((Pointable*)data)->pointers.end() : NULL) {
      if (data) {
	((Pointable*)data)->pointers.push_back((Pointer<Pointable>*)this);
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
	it(data ? ((Pointable*)data)->pointers.end() : NULL) {
      if (data) {
	((Pointable*)data)->pointers.push_back((Pointer<Pointable>*)this);
      }
    }

    template <typename U>
    explicit Pointer(Pointer<U>&& ptr)
      : data((T*) ptr.data),
	it(ptr.it) {
      *it = (Pointer<Pointable>*) this;
      ptr.data = NULL;
    }
  
    ~Pointer() {
      if (*this) {
	((Pointable*)data)->pointers.removeAndReplace(it);
	/*if (((Pointable*)data)->pointers.size() > 0) {
	  (*it)->it = it;
	  }*/
      }
    }
    
    Pointer& operator=(const Pointer& ptr) {
      data = ptr.data;
      if (data) {
	it = ((Pointable*)data)->pointers.end();
	((Pointable*)data)->pointers.push_back((Pointer<Pointable>*)this);
      } else {
	it = NULL;
      }
      return *this;
    }

    Pointer& operator=(Pointer&& ptr) {
      data = ptr.data;
      if (data) {
	it = ptr.it;
	*it = (Pointer<Pointable>*) this;
	ptr.data = NULL;
      }
      return *this;
    }

    void operator==(T* _data) const {
      return data == _data;
    }

    bool operator!=(T* _data) const {
      return data != _data;
    }

    operator bool() const {
      return data;
    }

    operator T*() const {
      return data;
    }
  
    T& operator*() const {
      debugAssert(data,
		  "You are trying to dereference a NULL pointer");
      return *data;
    }

    T* operator->() const {
      debugAssert(data,
		  "You are trying to dereference a NULL pointer");
      return data;
    }
    
  };

}
