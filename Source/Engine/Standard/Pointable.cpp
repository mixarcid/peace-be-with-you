#include "Pointable.hpp"
#include "Log.hpp"

NAMESPACE {

  Pointable::Pointable(const Pointable& p) {
  }

  Pointable::Pointable(Pointable&& p) {
    /*for (Pointer<Pointable>* pointer : p.pointers) {
      *pointer = this;
    }
    ensureCorrect();
    p.pointers.clear();*/
  }

  Pointable::~Pointable() {
    for (Pointer<Pointable>* pointer : pointers) {
      *pointer = NULL;
    }
  }
  
  void Pointable::operator=(const Pointable& p) {
  }

  void Pointable::operator=(Pointable&& p) {
  }

  Pointable::IndexType Pointable::addPointer(Pointer<Pointable>* ptr) {
    ensureCorrect();
    pointers.push_back(ptr);
    return (pointers.size() - 1);
  }
  
  void Pointable::setPointer(IndexType index, Pointer<Pointable>* ptr) {
    ensureCorrect();
    pointers[index] = ptr;
    ensureCorrect();
  }
  
  void Pointable::removePointer(IndexType index, Pointer<Pointable>* ptr) {
    ensureCorrect();
    for (auto& p : pointers) {
      Log::message("before: %p", p);
    }
    debugAssert(pointers[index] == ptr, "Ack");
    pointers.removeAndReplace(pointers.begin() + index);
    if (index < pointers.size()) {
      pointers[index]->index = index;
    }
    ensureCorrect();
    for (auto& p : pointers) {
      Log::message("after: %p", p);
    }
  }

  void Pointable::_on_move() {
    ensureCorrect();
    for (Pointer<Pointable>** pointer = this->pointers.begin();
	 pointer != this->pointers.end();
	 ++pointer) {
      (*pointer)->data = this;
    }
    ensureCorrect();
  }

  void Pointable::ensureCorrect() {
    u8 n = 0;
    for (Pointer<Pointable>* pointer : pointers) {
      debugAssert(pointer->index == (n++),
		  "Incorrect index!");
    }
  }

}
