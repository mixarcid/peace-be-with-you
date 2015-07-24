#define GLM_FORCE_RADIANS
#include "Camera.hpp"
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

NAMESPACE {

  Camera::Camera(Vec3f cam_pos, Vec3f cam_dir, Vec3f cam_up,
		 f32 cam_fovy, f32 cam_near, f32 cam_far)
    : pos(cam_pos), dir(cam_dir), up(cam_up),
    fovy(cam_fovy), near_clip(cam_near), far_clip(cam_far) {}

  Mat4f Camera::getModel() {
    Mat4f mat =  Mat4f::makeTranslate(-pos);
    return mat;
  }

  Mat4f Camera::getView() {
    
    Mat4f mat =  Mat4f::lookAt(Vec3f(0,0,0), dir, up);
    return mat;
  }

  Mat4f Camera::getProj() {
    Mat4f mat = Mat4f::perspective(fovy, aspect,
				   near_clip, far_clip);
    return mat;
  }

  void Camera::onWindowResize(i32 width, i32 height) {
    aspect = width / (f32) height;
    glViewport(0,0,width,height);
  }

}

