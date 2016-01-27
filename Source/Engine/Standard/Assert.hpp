#pragma once

#ifndef NDEBUG

NAMESPACE {
  void _assert_print(const char* message, ...);
  void _assert_print(String message);
}

#define debugAssert(cond, ...) do {			\
    if (!(cond)) {					\
      _assert_print(__VA_ARGS__);			\
      fprintf(stderr, "Stack trace:\n %s",		\
	      getStackTrace().c_str());			\
      abort();						\
    }							\
  } while (0)

#define fatalAssert(cond, ...)			\
  debugAssert(cond, __VA_ARGS__);

#else
    
#define debugAssert(cond, ...)
#define fatalAssert(cond, ...) do {		\
    if (!(cond)) {				\
      Log::fatalError(__VA_ARGS__);		\
    }						\
  } while (0)
#endif
