#pragma once

#include "Macros.hpp"
//#include <typeinfo>

#define TYPE_ID(classname) __type_id_##classname

#define RTTI_STRUCT(classname)			\
  const TypeId TYPE_ID(classname) = __COUNTER__;	\
  struct classname
#define RTTI_CONSTRUCTOR(classname)		\
  BaseRTTI::type_id = TYPE_ID(classname);


NAMESPACE {

  typedef u32 TypeId;
  
  struct BaseRTTI {
    TypeId type_id;
  };
  
}
