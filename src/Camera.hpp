#pragma once

#include "VectorMath.hpp"

NAMESPACE {

  struct Camera {

    Vec3f pos;
    Vec3f dir;
    Vec3f up;

    float fovy; //in radians, naturally
    float near_clip;
    float far_clip;

    int win_width;
    int win_height;
    float aspect;
    
    Camera(float cam_fovy, float cam_near, float cam_far);
    
    void onWindowResize(int width, int height);
    /* void setHeadingAbs(float radians);
    void setHeadingRel(float radians);
    void setPitchAbs(float radians);
    void setPitchRel(float radians);
    void setTransAbs(Vec3f trans);
    void setTransRel(Vec3f trans);*/
    
    Mat4f getModel();
    Mat4f getView();
    Mat4f getProj();

  };

}
