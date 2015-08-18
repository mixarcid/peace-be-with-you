#include "Camera.hpp"
#include "GL.hpp"

/*#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>*/

NAMESPACE {

  Camera::Camera(f32 cam_fovy,
		 f32 cam_near,
		 f32 cam_far)
    : fovy(cam_fovy),
    near_clip(cam_near),
    far_clip(cam_far) {}

  /*void Camera::translateAbs(Vec3f trans) {
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
    }*/

  Mat3f Camera::getCoord() {
    return rot.mat3();
  }

  /*const Mat4f CAM_BASE_ROT
    = Quaternionf(degreesToRadians(-90),0,0).mat4();*/
  Mat4f Camera::getView() {
    //Mat4f mat = rot.mat4();
    Mat3f coord = getCoord();
    Vec3f dir = coord.col(1);
    Vec3f up = coord.col(2);
    Mat4f mat = Mat4f::lookAt(trans,
			      trans + dir,
			      up);
    //Log::message(to_string(mat));
    //Log::message(to_string(rot.mat3()));
    return mat;
  }

  Mat4f Camera::getProj() {
    Mat4f mat = Mat4f::perspective(fovy,
				   aspect,
				   near_clip,
				   far_clip);
    //Log::message(to_string(mat));
    //Log::message(glm::to_string(glm::perspective(fovy,aspect,near_clip,far_clip)));
    return mat;
  }

  void Camera::onWindowResize(i32 width, i32 height) {
    aspect = width / (f32) height;
    glViewport(0,0,width,height);
  }

}

