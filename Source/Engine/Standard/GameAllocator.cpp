#include "GameAllocator.hpp"
#include "Log.hpp"

void* operator new(size_t size) {
  if (peace::GameAllocator::flags & peace::ALLOCATOR_INIT) {
    return peace::GameAllocator::malloc(size);
  } else {
    return malloc(size);
  }
}

void operator delete(void* ptr) noexcept {
  if (peace::GameAllocator::flags & peace::ALLOCATOR_INIT) {
  peace::GameAllocator::free(ptr);
  } else {
    free(ptr);
  }
}

void* operator new[](size_t size) {
  if (peace::GameAllocator::flags & peace::ALLOCATOR_INIT) {
    return peace::GameAllocator::malloc(size);
  } else {
    return malloc(size);
  }
}

 void operator delete[](void* ptr) noexcept {
   if (peace::GameAllocator::flags & peace::ALLOCATOR_INIT) {
     peace::GameAllocator::free(ptr);
   } else {
     free(ptr);
   }
 }
