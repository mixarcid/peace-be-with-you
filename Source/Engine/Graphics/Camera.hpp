#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"
#include "GameObject.hpp"

NAMESPACE {

  struct CameraComp : Component {
    
    f32 fovy; //in radians, naturally
    f32 near_clip;
    f32 far_clip;

    i32 win_width;
    i32 win_height;
    f32 aspect;

    CameraComp(f32 _fovy,
	       f32 _near,
	       f32 _far)
      : fovy(_fovy),
	near_clip(_near),
	far_clip(_far) {
      $rttiConstruct("CameraComp");
    }
  };
  $registerRttiStruct();
  
  struct Camera : DynamicObject {
    
    Camera(f32 _fovy = degreesToRadians(60),
	   f32 _near = 1,
	   f32 _far = 5000)
      : DynamicObject(Vec3f()) {
      $rttiConstruct("Camera");
      addComponent(new CameraComp(_fovy, _near, _far));
    }
    ~Camera() {
      delete getComponent<CameraComp>();
    }

    Mat3f getCoord();

    Mat4f getView();
    Mat4f getProj();

    void setAspect(Vec2i win_size);

  };
  $registerRttiStruct();

}
