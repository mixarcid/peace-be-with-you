#include "Camera.hpp"
#include "GL.hpp"

NAMESPACE {

  Camera::Camera(f32 cam_fovy,
		 f32 cam_near,
		 f32 cam_far)
    : fovy(cam_fovy),
    near_clip(cam_near),
    far_clip(cam_far) {}
  
  Mat3f Camera::getCoord() {
    return rot.mat3();
  }

  Mat4f Camera::getView() {
    Mat3f coord = getCoord();
    Vec3f dir = coord.col(1);
    Vec3f up = coord.col(2);
    Mat4f mat = Mat4f::lookAt(trans,
			      trans + dir,
			      up);
    return mat;
  }

  Mat4f Camera::getProj() {
    Mat4f mat = Mat4f::perspective(fovy,
				   aspect,
				   near_clip,
				   far_clip);
    return mat;
  }

  void Camera::onWindowResize(i32 width, i32 height) {
    aspect = width / (f32) height;
    glViewport(0,0,width,height);
  }

}

