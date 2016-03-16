#include "Camera.hpp"
#include "GL.hpp"

NAMESPACE {
  
  Mat3f Camera::getCoord() {
    return getRot().mat3();
  }

  Mat4f Camera::getView() {
    Mat3f coord = getCoord();
    Vec3f dir = coord.col(1);
    Vec3f up = coord.col(2);
    Mat4f mat = Mat4f::lookAt(getTrans(),
			      getTrans() + dir,
			      up);
    return mat;
  }

  Mat4f Camera::getProj() {
    Pointer<CameraComp> c = getComponent<CameraComp>();
    Mat4f mat = Mat4f::perspective(c->fovy,
				   c->aspect,
				   c->near_clip,
				   c->far_clip);
    return mat;
  }

  void Camera::setAspect(Vec2i win_size) {
    Pointer<CameraComp> c = getComponent<CameraComp>();
    c->aspect = win_size.x() / (f32) win_size.y();
  }

}

