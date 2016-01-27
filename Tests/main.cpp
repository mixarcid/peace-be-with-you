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

  bool test(TestPointable& a, TestPointable& b) {
    return a.data > b.data;
  }

}

i32 main() {

  GameAllocator::init();
  Log::init(NULL);
  SystemManager man;
  man.addSystem(Log::getSystem());
  man.start();
  
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
  printf("m: %s\n", to_string(m).c_str());
  printf("m2: %s\n", to_string(m2).c_str());
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

  printf("\nString tests\n");

  String str = "Hello there\n";
  String str2 = "Hello there\n";
  String str3 = "Hello!\n";
  printf("str: %s\n", str.c_str());		
  printf("str2: %s\n", str2.c_str());
  printf("str+str2: %s\n", (str+str2).c_str());
  printf("str == str2: %d\n", str == str2);
  printf("str == str3: %d\n", str == str3);

  printf("\nMyArray tests\n");

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
    printf("array value: %s\n", s.c_str());
  }

  printf("\nPointer tests\n");

  Array<TestPointable> points;
  
  points.emplace_back(5);
  points.insertSorted(TestPointable(3), test);
  points.insertSorted(TestPointable(7), test);
  points.insertSorted(TestPointable(6), test);
  points.insertSorted(TestPointable(2), test);
  points.insertSorted(TestPointable(4), test);

  Pointer<TestPointable> p = &points[3];

  Array<TestPointable> points2(std::move(points));
  Array<TestPointable> points3 = points2;

  for (TestPointable p : points2) {
    printf("TestPointable value: %d\n", p.data);
  }

  printf("\nTests complete\n");
  Log::terminate();
  GameAllocator::terminate();
  return 0;
}
