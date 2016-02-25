#pragma once

#include <stdio.h>
#include <stdlib.h>
$py(
#include "Macros.py"
    )
$extend(peaceEnum)
$extend(getEnumEntries)

#define NAMESPACE namespace peace

#define FOR $for
#define END_FOR $endfor

#define PEACE_STRING(x) #x

#define PEACE_UNIMPLIMENTED(ret)				\
  Log::fatalError("unimplimented function %s", __func__);	\
  return ret;

#define PEACE_ENUM(name, ...)		\
  $peaceEnum(#name, #__VA_ARGS__)

#define PEACE_DEFINE_BITFLAGS(name, size, ...)	\
  $peaceEnum(#name, #__VA_ARGS__, size);			\
  constexpr inline name operator~(const name a) {		\
    return (name) ~ (u##size) a;				\
  }								\
  constexpr inline name operator|(const name a, const name b) {	\
    return (name) ((u##size) a |  (u##size) b);			\
  }								\
  constexpr inline name operator&(const name a, const name b) {	\
    return (name) ((u##size) a & (u##size) b);			\
  }								\
  constexpr inline void operator|=(name& a, const name b) {	\
    a = a | b;							\
  }								\
  constexpr inline void operator&=(name& a, const name b) {	\
    a = a & b;							\
  }
