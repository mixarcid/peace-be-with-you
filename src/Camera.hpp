#pragma once

#include "VectorMath.hpp"
#include "Quaternion.hpp"

NAMESPACE {

  struct Camera {

    Vec3f pos;
    Mat3f rot;
    
    f32 fovy; //in radians, naturally
    f32 near_clip;
    f32 far_clip;

    i32 win_width;
    i32 win_height;
    f32 aspect;
    
    Camera(f32 cam_fovy, f32 cam_near, f32 cam_far);

    void translateAbs(Vec3f trans);
    void translateRel(Vec3f trans);
    void rotateAbs(Quaternionf q);
    void rotateRel(Quaternionf q);

    Vec3f getRight();
    Vec3f getDir();
    Vec3f getUp();

    Mat4f getModel();
    Mat4f getView();
    Mat4f getProj();

    void onWindowResize(i32 width, i32 height);

  };

}
