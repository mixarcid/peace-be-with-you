#pragma once

#include <stdio.h>
#include <stdlib.h>

#define NAMESPACE namespace peace

#ifndef NDEBUG

#define debugAssert(cond, ...) do {	\
    if (!(cond)) {				\
      log::exitError(__VA_ARGS__);			\
    }						\
  } while (0)
    
#define exitAssert(cond, ...) \
    debugAssert(cond, __VA_ARGS__);

#define fatalAssert(cond, ...) \
    debugAssert(cond, __VA_ARGS__);

#else
    
#define debugAssert(cond, ...)

#define exitAssert(cond, ...) do {	\
    if (!(cond)) {				\
      log::exitError(__VA_ARGS__);			\
    }						\
  } while (0)

#define fatalAssert(cond, ...) do {	\
    if (!(cond)) {				\
      log::fatalError(__VA_ARGS__);		\
    }						\
  } while (0)
#endif
