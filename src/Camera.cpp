#include "Camera.hpp"

NAMESPACE {

  Camera::Camera(Vec3f cam_pos, Vec3f cam_dir, Vec3f cam_up,
		 f32 cam_fovy, f32 cam_near, f32 cam_far)
    : pos(cam_pos), dir(cam_dir), up(cam_up),
    fovy(cam_fovy), near_clip(cam_near), far_clip(cam_far) {}

  Mat4f Camera::getModel() {
    return Mat4f::makeTranslate(-pos);
  }

  Mat4f Camera::getView() {
    return Mat4f::lookAt(pos, pos + dir, up);
  }

  Mat4f Camera::getProj() {
    return Mat4f::perspective(fovy, aspect,
			      near_clip, far_clip);
  }

  void Camera::onWindowResize(i32 width, i32 height) {
    aspect = width / (f32) height;
    glViewport(0,0,width,height);
  }

}
