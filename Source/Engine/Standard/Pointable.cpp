#include "Pointable.hpp"

NAMESPACE {

  Pointable::Pointable(const Pointable& p) {
    
  }

  Pointable::Pointable(Pointable&& p) {
    for (Pointer<Pointable>* pointer : p.pointers) {
      *pointer = this;
      pointers.push_back(pointer);
    }
    p.pointers.clear();
  }

  Pointable::~Pointable() {
    for (Pointer<Pointable>* pointer : pointers) {
      *pointer = NULL;
    }
  }
  
  void Pointable::operator=(const Pointable& p) {
    
  }

  void Pointable::operator=(Pointable&& p) {
    for (Pointer<Pointable>* pointer : pointers) {
      *pointer = NULL;
    }
    pointers.clear();
    for (Pointer<Pointable>* pointer : p.pointers) {
      pointer->data = this;
      pointers.push_back(pointer);
    }
    p.pointers.clear();
  }

  template<>
    void onMove(Pointable* ptr) {
    for (Pointer<Pointable>** pointer = ptr->pointers.begin();
	 pointer < ptr->pointers.end();
	 ++pointer) {
      (*pointer)->data = ptr;
    }
  }

  template<>
    void onMove(Pointer<Pointable>** ptr) {
    (*ptr)->it = ptr;
  }

}
