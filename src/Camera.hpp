#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"
#include "Transform.hpp"

NAMESPACE {

  struct Camera : Transform {
    
    f32 fovy; //in radians, naturally
    f32 near_clip;
    f32 far_clip;

    i32 win_width;
    i32 win_height;
    f32 aspect;
    
    Camera(f32 cam_fovy, f32 cam_near, f32 cam_far);

    Mat3f getCoord();

    Mat4f getView();
    Mat4f getProj();

    void onWindowResize(i32 width, i32 height);

  };

}
