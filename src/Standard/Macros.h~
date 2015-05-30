#pragma once

#include <cstdio>
#include <cstdlib>

#define NAMESPACE namespace peace

#ifndef NDEBUG
#define assert(cond, message) do { \
    if (!(cond)) { \
      printf("Assertion Error in %s:%d: %s\n", __FILE__, __LINE__, message); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)
#else
#define assert(cond, message) ;
#endif
