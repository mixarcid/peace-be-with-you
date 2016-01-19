#include "Allocator.hpp"
#include "Log.hpp"
#include "Exception.hpp"

NAMESPACE {

  inline void* _malloc(size_t size) {
    return malloc(size);
  }

  inline void* _calloc(size_t num, size_t size) {
    return calloc(num, size);
  }

  inline void* _realloc(void* ptr, size_t size) {
    return realloc(ptr, size);
  }

  inline void _free(void* ptr) {
    free(ptr);
  }

  u64 Allocator::num_allocations = 0;
  AllocatorFlags Allocator::flags = ALLOCATOR_NO_FLAGS;
  
  void Allocator::init() {
    flags = ALLOCATOR_INIT;
  }
  
  void Allocator::terminate() {
    debugAssert(num_allocations==0,
		"You have %llu memory leaks",
		num_allocations);
    flags = ALLOCATOR_NO_FLAGS;
  }
  
  void* Allocator::malloc(size_t size) {
    return _malloc(size);
  }
  
  void* Allocator::calloc(size_t num, size_t size) {
    return _calloc(num, size);
  }

  void* Allocator::realloc(void* ptr, size_t size) {
    return _realloc(ptr, size);
  }
  
  void Allocator::free(void* ptr) {
    return _free(ptr);
  }

}
