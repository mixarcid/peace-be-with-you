#include "BoundingObjectUnion.hpp"

NAMESPACE {

  BoundingObjectUnion::BoundingObjectUnion()
    : object() {}

  BoundingObject* BoundingObjectUnion::getBoundingObject() {
    return &object;
  }

  void BoundingObjectUnion::set(BoundingObject* obj) {

    switch(obj->type) {
      FOR(entry in getEnumEntries("BoundingObjectType")[:-2]);
    case BoundingObject::$(entry):
      new (&obj_$(entry)) Bounding$(className(entry))
	(*((Bounding$(className(entry))*) obj));
      return;
      END_FOR;
    default:
      return;
    }
  }
      
}
