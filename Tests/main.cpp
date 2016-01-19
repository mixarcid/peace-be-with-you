#include "Vector.hpp"
#include "Matrix.hpp"
#include "RTTI.hpp"

#define st(x) #x

using namespace peace;

NAMESPACE {

  struct A : BaseRTTI {
    A() {
      $rttiConstruct("A");
    }
  };
  $registerRttiStruct();

  struct B : A {
    B() {
      $rttiConstruct("B");
    }
  };
  $registerRttiStruct();

}

i32 main() {

  //GameAllocator::init();
  Log::init(NULL);
  SystemManager man({&Log::logger});
  man.start();
  
  Log::message("Testing vector libraries...");
  
  Log::message("Vectors:");
  
  Vec3f v1(1,2,3);
  Vec3f v2(6,2,10);
  Log::message("v1: %s", to_string(v1).c_str());
  Log::message("x: %f, abs: %f", v1.x(), v1.norm());
  Log::message("v1 dot v2: %f", Vec3f::dot(v1, v2));
  Log::message("v1 cross v2: %s", to_string(Vec3f::cross(v1, v2)).c_str());

  Log::message("Matrices:");

  Mat4f m;
  f32 m_data[9] = {1,2,3,
		   4,5,6,
		   7,8,9};
  Mat3f m2(m_data);
  Mat4f t = Mat4f::translate(v1);
  Mat4f s = Mat4f::scale(v2);
  Log::message("m: %s", to_string(m).c_str());
  Log::message("m2: %s", to_string(m2).c_str());
  Log::message("m2 row 1: %s", to_string(m2.row(0)).c_str());
  Log::message("m2 col 2: %s", to_string(m2.col(1)).c_str());
  Log::message("m2*v1: %s", (to_string(m2*v1)).c_str());
  Log::message("t: %s", (to_string(t)).c_str());
  Log::message("s: %s", (to_string(s)).c_str());
  Log::message("m2*m2: %s", (to_string(m2*m2)).c_str());

  Log::message("RTTI tests");

  A a;
  B b;
  Log::message("a is A: %d", typeId(&a) == typeId<A>());
  Log::message("b is B: %d", typeId(&b) == typeId<B>());
  Log::message("a is b: %d", typeId(&a) == typeId(&b));

  Log::message("Array tests");
  Array<int> arr = {100,200,300,400,10000};
  Log::message("arr values:");
  for (int i : arr) {
    Log::message("   %i", i);
  }
  Log::message("arr[4] = %i", arr[4]);
  
  Log::message("Tests complete");
  Log::terminate();
  //GameAllocator::terminate();
  return 0;
}
