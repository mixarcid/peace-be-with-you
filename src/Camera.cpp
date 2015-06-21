#include "Camera.hpp"

NAMESPACE {

  Camera::Camera(Vec3f cam_pos, Vec3f cam_dir, Vec3f cam_up,
		 float cam_fovy, float cam_near, float cam_far)
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

  void Camera::onWindowResize(int width, int height) {
    aspect = width / (float) height;
    glViewport(0,0,width,height);
  }

}
