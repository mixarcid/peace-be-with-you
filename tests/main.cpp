#include "Vector.hpp"
#include "Matrix.hpp"
#include "RTTI.hpp"

#define st(x) #x

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

using namespace peace;

i32 main() {
  printf("Testing vector libraries...\n");
  
  printf("\nVectors:\n");
  
  Vec3f v1(1,2,3);
  Vec3f v2(6,2,10);
  printf("v1: %s\n", to_string(v1).c_str());
  printf("x: %f, abs: %f\n", v1.x(), v1.norm());
  printf("v1 dot v2: %f\n", Vec3f::dot(v1, v2));
  printf("v1 cross v2: %s\n", to_string(Vec3f::cross(v1, v2)).c_str());

  printf("\nMatrices:\n");

  Mat4f m;
  f32 m_data[9] = {1,2,3,
		   4,5,6,
		   7,8,9};
  Mat3f m2(m_data);
  Mat4f t = Mat4f::translate(v1);
  Mat4f s = Mat4f::scale(v2);
  printf("m: %s", to_string(m).c_str());
  printf("m2: %s", to_string(m2).c_str());
  printf("m2 row 1: %s\n", to_string(m2.row(0)).c_str());
  printf("m2 col 2: %s\n", to_string(m2.col(1)).c_str());
  printf("m2*v1: %s\n", (to_string(m2*v1)).c_str());
  printf("t: %s\n", (to_string(t)).c_str());
  printf("s: %s\n", (to_string(s)).c_str());
  printf("m2*m2: %s\n", (to_string(m2*m2)).c_str());

  printf("\nRTTI tests\n");

  A a;
  B b;
  printf("a is A: %d\n", typeId(&a) == typeId<A>());
  printf("b is B: %d\n", typeId(&b) == typeId<B>());
  printf("a is b: %d\n", typeId(&a) == typeId(&b));
  
  printf("\nTests complete\n");
  return 0;
}
