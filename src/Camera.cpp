#include "Camera.hpp"

NAMESPACE {

  Camera::Camera(f32 cam_fovy,
		 f32 cam_near,
		 f32 cam_far)
    : fovy(cam_fovy),
    near_clip(cam_near),
    far_clip(cam_far) {}

  void Camera::translateAbs(Vec3f trans) {
    pos = trans;
  }
  
  void Camera::translateRel(Vec3f trans) {
    pos += trans;
  }
  
  void Camera::rotateAbs(Quaternionf q) {
    rot = q.getMat3();
  }
  
  void Camera::rotateRel(Quaternionf q) {
    rot *= q.getMat3();
  }

  Vec3f Camera::getRight() {
    return rot[0];
  }

  Vec3f Camera::getDir() {
    return rot[1];
  }

  Vec3f Camera::getUp() {
    return rot[2];
  }
    
  Mat4f Camera::getModel() {
    Mat4f mat =  Mat4f::makeTranslate(-pos);
    return mat;
  }

  Mat4f Camera::getView() {
    Mat4f mat(rot, pos);
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

