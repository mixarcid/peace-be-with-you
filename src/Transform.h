#pragma once

#include "Standard.h"
#include "VectorMath.h"

NAMESPACE {

  struct Rotate {
    
    Vec3f axis;
    float angle; //in radians

    Rotate();
  };

  /*calling the set* functions will undo (probably strangly)
    compound transformations*/
  struct Transform {

    Vec3f trans;
    Vec3f scal;
    Rotate rot;
    Mat4f mat;

    Transform();
    Transform(Mat4f matrix);

    void setTranslateAbs(Vec3f trans_v);
    void setTranslateRel(Vec3f trans_v);
    void translate(Vec3f trans_v);

    void setRotateAbs(Vec3f axis, float angle);
    void setRotateRel(Vec3f axis, float angle);
    void rotate(Vec3f axis, float angle);
    
    void setScaleAbs(Vec3f scale_v);
    void setScaleRel(Vec3f scale_v);
    void scale(Vec3f scale_v);

    //flush the matrix after you've called your set* functions
    void flush();

    void combine(Transform b);
    
    void use();
    
    static Transform combine(Transform a, Transform b);
    
  };

}
