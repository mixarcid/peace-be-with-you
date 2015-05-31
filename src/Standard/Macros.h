#pragma once

#include <cstdio>
#include <cstdlib>

#define NAMESPACE namespace peace

#ifndef NDEBUG

#define debugAssert(cond, message) do {					\
    if (!(cond)) {							\
      printf("Assertion Error in %s:%d: %s\n", __FILE__, __LINE__, message); \
      exit(EXIT_FAILURE);						\
    }									\
  } while (0)

#define exitAssert(cond, message) do {		\
    if (!(cond)) {				\
      log::exitError(message);			\
    }						\
  } while (0)

#define fatalAssert(cond, message) do {		\
    if (!(cond)) {				\
      log::fatalError(message);			\
    }						\
  } while (0)

#else
#define debugAssert(cond, message) ;
#endif
