#pragma once

#include <stdio.h>
#include <stdlib.h>

#define NAMESPACE namespace peace

#ifndef NDEBUG

#define debugAssert(cond, ...) do {	\
    if (!(cond)) {				\
      Log::fatalError(__VA_ARGS__);			\
    }						\
  } while (0)

#define fatalAssert(cond, ...) \
    debugAssert(cond, __VA_ARGS__);

#else
    
#define debugAssert(cond, ...)
#define fatalAssert(cond, ...) do {	\
    if (!(cond)) {				\
      Log::fatalError(__VA_ARGS__);		\
    }						\
  } while (0)
#endif
