#pragma once

#include <stdio.h>
#include <stdlib.h>

#define NAMESPACE namespace peace

//thanks, adamk from Stack Overflow
#define PEACE_SWAP(x,y) do \
   { unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
     memcpy(swap_temp,&y,sizeof(x)); \
     memcpy(&y,&x,       sizeof(x)); \
     memcpy(&x,swap_temp,sizeof(x)); \
   } while(0)

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
