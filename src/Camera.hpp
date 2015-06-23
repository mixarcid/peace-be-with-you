#pragma once

#include "VectorMath.hpp"

NAMESPACE {

  struct Camera {

    Vec3f pos;
    Vec3f dir;
    Vec3f up;

    f32 fovy; //in radians, naturally
    f32 near_clip;
    f32 far_clip;

    i32 win_width;
    i32 win_height;
    f32 aspect;
    
    Camera(Vec3f cam_pos, Vec3f cam_dir, Vec3f cam_up,
	   f32 cam_fovy, f32 cam_near, f32 cam_far);
    
    void onWindowResize(i32 width, i32 height);
    /* void setHeadingAbs(f32 radians);
    void setHeadingRel(f32 radians);
    void setPitchAbs(f32 radians);
    void setPitchRel(f32 radians);
    void setTransAbs(Vec3f trans);
    void setTransRel(Vec3f trans);*/
    
    Mat4f getModel();
    Mat4f getView();
    Mat4f getProj();

  };

}
