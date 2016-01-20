#include "Pointable.hpp"

NAMESPACE {

  Pointable::Pointable(Pointable& p) {
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
    for (Pointer<Pointable>* pointer : pointers) {
      *pointer = this;
    }
  }

  void Pointable::operator=(Pointable& p) {
    
  }

  void Pointable::operator=(Pointable&& p) {
    for (Pointer<Pointable>* pointer : p.pointers) {
      *pointer = this;
      pointers.push_back(pointer);
    }
    p.pointers.clear();
  }

}
