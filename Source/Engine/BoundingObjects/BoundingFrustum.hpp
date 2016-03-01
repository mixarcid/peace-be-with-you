#pragma once

#include "BoundingObject.hpp"

NAMESPACE {

  struct BoundingFrustum : BoundingObjectBase<BoundingObject::FRUSTUM> {

    Vec4f planes[6];

    BoundingFrustum(Mat4f view_proj);
    
  };

}
