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
    inline void ensureCorrect();

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
      ensureCorrect();
    }

    Pointer(const Pointer& ptr)
      : data(ptr.data) {
      if (data) {
        index = data->addPointer((Pointer<Pointable>*)this);
      }
      ensureCorrect();
    }

    Pointer(Pointer&& ptr)
      : data(ptr.data),
        index(ptr.index) {
      if (data) {
        data->setPointer(index, (Pointer<Pointable>*)this);
      }
      ensureCorrect();
      ptr.data = NULL;
    }
  
    ~Pointer() {
      ensureCorrect();
      if (*this) {
        data->removePointer(index, (Pointer<Pointable>*)this);
      }
    }

    void _on_move() {
      if (data) {
	data->setPointer(index, (Pointer<Pointable>*)this);
	ensureCorrect();
      }
    }

    inline void ensureCorrect() {
#ifdef PEACE_POINTER_DEBUG
      if (*this) {
	debugAssert((data)->pointers[index] == (Pointer<Pointable>*)this, "Ack!");
      }
#endif
    }

    Pointer& operator=(const Pointer& ptr) {
      ensureCorrect();
      if (*this) {
        data->removePointer(index, (Pointer<Pointable>*) &ptr);
      }
      data = ptr.data;
      if (data) {
	index = data->addPointer((Pointer<Pointable>*)this);
      }
      ensureCorrect();
      return *this;
    }

    Pointer& operator=(Pointer&& ptr) {
      ensureCorrect();
      if (*this) {
	data->removePointer(index, (Pointer<Pointable>*) &ptr);
      }
      data = ptr.data;
      if (data) {
	index = ptr.index;
        data->setPointer(index, (Pointer<Pointable>*)this);
	ptr.data = NULL;
      }
      ensureCorrect();
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
