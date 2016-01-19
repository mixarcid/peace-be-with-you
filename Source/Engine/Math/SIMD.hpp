#pragma once

#include "Standard.hpp"

#if defined(__SSE__)				\
  || defined(__SSE2__)				\
  || defined(__SSE3__)				\
  || defined(__AVX__)				\
  || defined(__AVX2__)

#define SIMD

NAMESPACE {
#include <x86intrin.h>
}

#endif
