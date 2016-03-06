#include "GameAllocator.hpp"
#include "Log.hpp"

void* operator new(size_t size) {
  void* ret;
  if (peace::GameAllocator::flags & peace::ALLOCATOR_INIT) {
    ret = peace::GameAllocator::malloc(size);
  } else {
    ret = malloc(size);
  }
  if (!ret){
    peace::Log::fatalError("Allocation of size %llu failed", size);
  }
  return ret;
}

void operator delete(void* ptr) noexcept {
  if (peace::GameAllocator::flags & peace::ALLOCATOR_INIT) {
  peace::GameAllocator::free(ptr);
  } else {
    free(ptr);
  }
}

void* operator new[](size_t size) {
  void* ret;
  if (peace::GameAllocator::flags & peace::ALLOCATOR_INIT) {
    ret = peace::GameAllocator::malloc(size);
  } else {
    ret = malloc(size);
  }
  if (!ret){
    peace::Log::fatalError("Allocation of size %llu failed", size);
  }
  return ret;
}

 void operator delete[](void* ptr) noexcept {
   if (peace::GameAllocator::flags & peace::ALLOCATOR_INIT) {
     peace::GameAllocator::free(ptr);
   } else {
     free(ptr);
   }
 }
