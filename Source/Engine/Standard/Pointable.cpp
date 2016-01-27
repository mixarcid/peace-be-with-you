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

  void Pointable::onMove() {
    for (Pointer<Pointable>** pointer = pointers.begin();
	 pointer < pointers.end();
	 ++pointer) {
      (*pointer)->data = this;
      (*pointer)->it = pointer;
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
      pointer->it = pointers.end();
      pointers.push_back(pointer);
    }
    p.pointers.clear();
  }

}
