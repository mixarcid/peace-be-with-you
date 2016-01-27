#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include "Pointable.hpp"
#include "RTTI.py"


/*
  We can create an rtti class using:
  struct RTTI_class : BaseRTTI {
     //though it should not extend BaseRTTI if it already inherits it somehow
     RTTI_class() {
        $rttiConstruct("RTTI_class");
     }
  };
  $registerRttiStruct();
 */

NAMESPACE {

  typedef u32 TypeId;
  
  struct BaseRTTI {
    TypeId type_id;
    virtual ~BaseRTTI() {}
  };

  inline TypeId typeId(BaseRTTI* object) {
    return object->type_id;
  }

  template <typename T>
    inline TypeId typeId(Pointer<T> object) {
    return object->type_id;
  }

  template <typename T>
    inline TypeId typeId() {
    return 0;
  }
  
}
