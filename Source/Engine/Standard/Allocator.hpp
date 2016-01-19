#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include <stdlib.h>

NAMESPACE {

  PEACE_DEFINE_BITFLAGS(AllocatorFlags, 8,
			ALLOCATOR_NO_FLAGS = 0x00,
			ALLOCATOR_INIT = 0x01);
  
  struct Allocator {

    static u64 num_allocations;
    static AllocatorFlags flags;

    static void init();
    static void terminate();
    static void* malloc(size_t size);
    static void* calloc(size_t num, size_t size);
    static void* realloc(void* ptr, size_t size);
    static void free(void* ptr);

  };

}
