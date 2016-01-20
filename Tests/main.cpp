//#define __STDC_WANT_LIB_EXT1__ 1
#include <cstring>
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Pointable.hpp"
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

  struct TestPointable : Pointable {

    i32 data;

    TestPointable(i32 _data) : data(_data) {}
    
  };

}

i32 main() {

  GameAllocator::init();
  Log::init(NULL);
  SystemManager man;
  man.addSystem(&Log::logger);
  man.start();
  
  Log::message("Testing vector libraries...");
  
  Log::message("\nVectors:");
  
  Vec3f v1(1,2,3);
  Vec3f v2(6,2,10);
  Log::message("v1: %s", to_string(v1).c_str());
  Log::message("x: %f, abs: %f", v1.x(), v1.norm());
  Log::message("v1 dot v2: %f", Vec3f::dot(v1, v2));
  Log::message("v1 cross v2: %s", to_string(Vec3f::cross(v1, v2)).c_str());

  Log::message("\nMatrices:");

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

  Log::message("\nRTTI tests");

  A a;
  B b;
  Log::message("a is A: %d", typeId(&a) == typeId<A>());
  Log::message("b is B: %d", typeId(&b) == typeId<B>());
  Log::message("a is b: %d", typeId(&a) == typeId(&b));

  Log::message("\nString tests");

  String str = "Hello there";
  String str2 = "Hello there";
  String str3 = "Hello!";
  Log::message("str: %s", str.c_str());		
  Log::message("str2: %s", str2.c_str());
  Log::message("str+str2: %s", (str+str2).c_str());
  Log::message("str == str2: %d", str == str2);
  Log::message("str == str3: %d", str == str3);

  Log::message("\nArray tests");

  Array<String> arr;
  arr.push_back(str);
  arr.push_back(str2);
  arr.push_back(str3);
  arr.push_back(str);
  arr.push_back(str2);
  arr.push_back(str3);
  arr.push_back(str);
  arr.push_back(str2);
  arr.push_back(str3);
  arr.push_back(str);
  arr.push_back(str2);
  arr.push_back(str3);
  arr.push_back(str);
  arr.push_back(str2);
  arr.push_back(str3);
  arr.push_back(str);
  arr.push_back(str2);
  arr.push_back(str3);
  arr.push_back(str);
  arr.push_back(str2);
  arr.push_back(str3);
  arr.push_back(str);
  arr.push_back(str2);
  arr.push_back(str3);
  
  for (String s : arr) {
    Log::message("array value: %s", s.c_str());
  }

  Log::message("\nPointer tests");

  Array<TestPointable> points;
  points.emplace_back(10);
  points.emplace_back(11);
  points.emplace_back(12);
  points.emplace_back(13);
  Pointer<TestPointable> ptr(&points[2]);
  for (TestPointable p : points) {
    Log::message("TestPointable value: %d", p.data);
  }
  Log::message("ptr->data: %d", ptr->data);
  points.removeAndReplace(points.begin());
  points.removeAndReplace(points.begin());
  for (TestPointable p : points) {
    Log::message("TestPointable value: %d", p.data);
  }
  Log::message("ptr->data: %d", ptr->data);

  Log::message("\nTests complete");
  Log::terminate();
  GameAllocator::terminate();
  return 0;
}
