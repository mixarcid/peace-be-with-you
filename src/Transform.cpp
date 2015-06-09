#include "Transform.h"
#include "Shader.h"

NAMESPACE {

  Rotate::Rotate() : axis(0,0,1), angle(0) {}
  
  Transform::Transform() : scal(1,1,1) {}

  Transform::Transform(Mat4f matrix) : scal(1,1,1)  {
    mat = matrix;
  }

  void Transform::setTranslateAbs(Vec3f trans_v) {
    trans = trans_v;
  }
  void Transform::setTranslateRel(Vec3f trans_v) {
    trans += trans_v;
  }
  void Transform::translate(Vec3f trans_v) {
    mat.translate(trans_v);
  }

  void Transform::setRotateAbs(Vec3f axis, float angle) {
    rot.axis = axis;
    rot.angle = angle;
  }
  void Transform::setRotateRel(Vec3f axis, float angle) {
    rot.axis += axis;
    rot.angle += angle;
  }
  void Transform::rotate(Vec3f axis, float angle) {
    mat.rotate(axis, angle);
  }
    
  void Transform::setScaleAbs(Vec3f scale_v) {
    scal = scale_v;
  }
  void Transform::setScaleRel(Vec3f scale_v) {
    scal += scale_v;
  }
  void Transform::scale(Vec3f scale_v) {
    mat.scale(scale_v);
  }

  void Transform::flush() {
    mat = Mat4f::makeScale(scal);
    mat.rotate(rot.axis, rot.angle);
    mat.translate(trans);
  }

  void Transform::combine(Transform b) {
    mat *= b.mat;
  }
  
  void Transform::use() {
    Shader::UNI_MODEL.registerMat4f(mat);
  }

  Transform Transform::combine(Transform a, Transform b) {
    return Transform(a.mat * b.mat);
  }
  
}
