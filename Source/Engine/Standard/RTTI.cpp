#include "RTTI.hpp"

NAMESPACE {

  BaseRTTI::BaseRTTI()
    : type_id(0) {}
  BaseRTTI::BaseRTTI(BaseRTTI& b)
    : type_id(b.type_id) {}
  BaseRTTI::BaseRTTI(BaseRTTI&& b) 
    : type_id(b.type_id) {}
  BaseRTTI& BaseRTTI::operator=(BaseRTTI& b) {
    type_id = b.type_id;
    return *this;
  }
  BaseRTTI& BaseRTTI::operator=(BaseRTTI&& b) {
    type_id = b.type_id;
    return *this;
  }

};
