#pragma once

#include "Standard.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"
#include "Pointable.hpp"

NAMESPACE {

  template <typename T, u8 Pad>
    struct TransformTemp {

    Vec3f trans;
    f32 pad[Pad];
    Quaternionf rot;

    TransformTemp(Vec3f _trans = Vec3f(0,0,0),
	      Quaternionf _rot = Quaternionf(0,0,0,1))
      : trans(_trans), rot(_rot) {}

    Vec3f getTrans() {
      return trans;
    }
    
    Quaternionf getRot() {
      return rot;
    }
    
    Mat4f getMat() {
      return Mat4f::translate(trans)*rot.mat4();
    }

    template <typename U, typename V>
    static T combine(const U& a, const V& b) {
      T ret(a.trans + (a.rot * b.trans),
		    a.rot*b.rot);
      return ret;
    }

    template <typename U, typename... Args>
    static T combine(const U& a, const Args... args) {
      return combine(a, combine(args...));
    }

    template <typename U, typename V>
    static T interp(const U& a, const V& b, f32 h) {
      return T(Vec3f::lerp(a.trans, b.trans, h),
	       Quaternionf::lerp(a.rot, b.rot, h));
    }
    
  };

  struct Transform : TransformTemp<Transform, 0> {
    using TransformTemp::TransformTemp;
  };

  struct TransformPad : TransformTemp<TransformPad, 1> {
    using TransformTemp::TransformTemp;
  };
  
}
