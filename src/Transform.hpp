#pragma once

#include "Standard.hpp"
#include "VectorMath.hpp"
#include "Quaternion.hpp"

NAMESPACE {

  struct Transform {

    Vec3f trans;
    f32 pad[1];
    //Vec3f scal;
    Quaternionf rot;

    Transform(Vec3f translation = Vec3f(0,0,0),
	      Quaternionf rotation = Quaternionf(0,0,0,1));

    void translateAbs(Vec3f trans_v);
    void translateRel(Vec3f trans_v);

    void rotateAbs(Quaternionf q);
    void rotateRel(Quaternionf q);
    
    //void scaleAbs(Vec3f scale_v);
    //void scaleRel(Vec3f scale_v);
    
    Mat4f getMat();

    static Transform combine(Transform a, Transform b);
    static Transform interp(Transform a, Transform b, f32 h);
    
  };

}
