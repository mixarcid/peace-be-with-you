#include "Camera.hpp"

NAMESPACE {

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
  }

}
