#pragma once

#include <stdio.h>
#include <stdlib.h>

#define NAMESPACE namespace peace

#define PEACE_STRING(x) #x

#define PEACE_UNIMPLIMENTED(ret)				\
  Log::fatalError("unimplimented function %s", __func__);	\
  return ret;

#define PEACE_DEFINE_BITFLAGS(name, size, ...)	\
  enum name : u##size {				\
    __VA_ARGS__					\
      };					\
  inline name operator~(const name a) {		\
    return (name) ~ (u##size) a;		\
  }						\
  inline name operator|(const name a, const name b) {	\
    return (name) ((u##size) a |  (u##size) b);	\
  }						\
  inline name operator&(const name a, const name b) {	\
    return (name) ((u##size) a & (u##size) b);	\
  }						\
  inline void operator|=(name& a, const name b) {	\
    a = a | b;						\
  }							\
  inline void operator&=(name& a, const name b) {	\
    a = a & b;						\
  }
