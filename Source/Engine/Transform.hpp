#pragma once

#include "Standard.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"

NAMESPACE {

  template <u8 Pad>
    struct TransformTemp {

    Vec3f trans;
    f32 pad[Pad];
    Quaternionf rot;

    TransformTemp(Vec3f _trans = Vec3f(0,0,0),
	      Quaternionf _rot = Quaternionf(0,0,0,1))
      : trans(_trans), rot(_rot) {}
    
    Mat4f getMat() {
      return Mat4f::translate(trans)*rot.mat4();
    }

    template<typename T>
    static T combine(T a, T b) {
      T ret(a.trans + (a.rot * b.trans),
		    b.rot*a.rot);
      return ret;
    }

    template<typename T>
    static T interp(T a, T b, f32 h) {
      return T(Vec3f::lerp(a.trans, b.trans, h),
	       Quaternionf::lerp(a.rot, b.rot, h));
    }
    
  };

  struct Transform : TransformTemp<0> {
    using TransformTemp::TransformTemp;
  };
  struct TransformPad : TransformTemp<1> {
    using TransformTemp::TransformTemp;
  };

}
