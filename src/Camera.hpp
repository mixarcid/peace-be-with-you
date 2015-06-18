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

    void onWindowResize(int width, int height);
    Mat4f getModel();
    Mat4f getView();
    Mat4f getProj();

  };

}
