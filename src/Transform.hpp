#pragma once

#include "Standard.hpp"
#include "VectorMath.hpp"
#include "Quaternion.hpp"

NAMESPACE {

  struct Transform {

    Vec3f trans;
    Vec3f scal;
    Quaternionf rot;

    Transform();
    Transform(Mat4f matrix);

    void translateAbs(Vec3f trans_v);
    void translateRel(Vec3f trans_v);

    void rotateAbs(Quaternionf q);
    void rotateRel(Quaternionf q);
    
    void scaleAbs(Vec3f scale_v);
    void scaleRel(Vec3f scale_v);

    Mat4f getMat();
    
  };

}
