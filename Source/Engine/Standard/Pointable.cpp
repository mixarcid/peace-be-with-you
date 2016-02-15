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

  void Pointable::onMove() {
    for (Pointer<Pointable>** pointer = this->pointers.begin();
	 pointer < this->pointers.end();
	 ++pointer) {
      (*pointer)->data = this;
    }
  }

  template<>
    void onMove(Pointer<Pointable>** ptr) {
    (*ptr)->it = ptr;
  }

}
