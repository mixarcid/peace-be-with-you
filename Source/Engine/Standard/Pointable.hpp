#pragma once

#include "RTTI.hpp"
#include "Containers.hpp"

NAMESPACE {

  template <typename T>
    struct Pointer;

  struct Pointable : BaseRTTI {

    typedef Array<Pointer<Pointable>*>::SizeType IndexType;

    Array<Pointer<Pointable>*> pointers;

    Pointable() {}
    Pointable(const Pointable& p);
    Pointable(Pointable&& p);
    
    virtual ~Pointable();
    
    void operator=(const Pointable& p);
    void operator=(Pointable&& p);

    IndexType addPointer(Pointer<Pointable>* ptr);
    void setPointer(IndexType index, Pointer<Pointable>* ptr);
    void removePointer(IndexType index, Pointer<Pointable>* ptr);
    void _on_move();
    void ensureCorrect();

  };

  template <typename T>
    struct Pointer {

    T* data;
    Pointable::IndexType index;

    Pointer(T* _data = NULL)
      : data(_data) {
      if (data) {
	index = data->addPointer((Pointer<Pointable>*)this);
      }
      if (*this) {
	debugAssert((data)->pointers[index] == (Pointer<Pointable>*)this, "Ack!");
      }
    }

    Pointer(const Pointer& ptr)
      : data(ptr.data) {
      if (data) {
        index = data->addPointer((Pointer<Pointable>*)this);
      }
      	if (*this) {
	  debugAssert((data)->pointers[index] == (Pointer<Pointable>*)this, "Ack!");
	}
    }

    Pointer(Pointer&& ptr)
      : data(ptr.data),
        index(ptr.index) {
      if (data) {
        data->setPointer(index, (Pointer<Pointable>*)this);
      }
      	if (*this) {
	  debugAssert((data)->pointers[index] == (Pointer<Pointable>*)this, "Ack!");
	}
      ptr.data = NULL;
    }

    /*template <typename U>
    explicit Pointer(const Pointer<U>& ptr)
      : data((T*)ptr.data) {
      if (data) {
	index = data->addPointer((Pointer<Pointable>*)this);
      }
      	if (*this) {
	  debugAssert((data)->pointers[index] == (Pointer<Pointable>*)this, "Ack!");
	}
    }

    template <typename U>
    explicit Pointer(Pointer<U>&& ptr)
      : data((T*)ptr.data),
	index(ptr.index) {
      if (data) {
	data->setPointer(index, (Pointer<Pointable>*)this);
      }
      	if (*this) {
	  debugAssert((data)->pointers[index] == (Pointer<Pointable>*)this, "Ack!");
	}
      ptr.data = NULL;
      }*/

  
    ~Pointer() {
      if (*this) {
	debugAssert((data)->pointers[index] == (Pointer<Pointable>*)this, "Ack!");
      }
      if (*this) {
        data->removePointer(index, (Pointer<Pointable>*)this);
      }
    }
    
    Pointer& operator=(const Pointer& ptr) {
      	if (*this) {
	  debugAssert((data)->pointers[index] == (Pointer<Pointable>*)this, "Ack!");
	}
      if (*this) {
        data->removePointer(index);
      }
      data = ptr.data;
      if (data) {
	index = data->addPointer((Pointer<Pointable>*)this);
      }
      	if (*this) {
	  debugAssert((data)->pointers[index] == (Pointer<Pointable>*)this, "Ack!");
	}
      return *this;
    }

    void _on_move() {
      data->setPointer(index, (Pointer<Pointable>*)this);
      if (*this) {
	debugAssert((data)->pointers[index] == (Pointer<Pointable>*)this, "Ack!");
      }
    }

    Pointer& operator=(Pointer&& ptr) {
      	if (*this) {
	  debugAssert((data)->pointers[index] == (Pointer<Pointable>*)this, "Ack!");
	}
      if (*this) {
	data->removePointer(index, (Pointer<Pointable>*) &ptr);
      }
      data = ptr.data;
      if (data) {
	index = ptr.index;
        data->setPointer(index, (Pointer<Pointable>*)this);
	ptr.data = NULL;
      }
      	if (*this) {
	  debugAssert((data)->pointers[index] == (Pointer<Pointable>*)this, "Ack!");
	}
      return *this;
    }

    bool operator==(const Pointer& b) {
      return data == b.data;
    }
    
    bool operator==(Pointable* _data) const {
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

    template <typename U>
    explicit operator Pointer<U>&() {
      return *((Pointer<U>*)this);
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

    static Pointer<T> null;
    
  };

  template<typename T>
    Pointer<T> Pointer<T>::null;
  
  template <typename T>
    inline TypeId typeId(Pointer<T> object) {
    return object->type_id;
  }
}
