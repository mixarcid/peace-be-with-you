#pragma once

#include <unordered_map>
#include <initializer_list>
#include <string.h>
#include "Types.hpp"
#include "Function.hpp"
#include "GameAllocator.hpp"
#include "Exception.hpp"

NAMESPACE {

  template <typename Key, typename Val>
    using HashMap = typename std::unordered_map<Key, Val>;
  template <typename A, typename B>
    using Pair = typename std::pair<A, B>;
  template <typename T>
    using InitList = typename std::initializer_list<T>;

  //Thanks, yrp from StackOverflow!
  template <typename T>
    struct HasOnMove {
    
    /* SFINAE foo-has-correct-sig :) */
    template<typename A>
    static std::true_type test(void(A::*)(A*)) {
      return std::true_type();
    }

    /* SFINAE foo-exists :) */
    template <typename A> 
    static decltype(test(&A::onMove)) 
    test(decltype(&A::onMove),void *) {
      /* foo exists. What about sig? */
      typedef decltype(test(&A::onMove)) return_type; 
      return return_type();
    }

    /* SFINAE game over :( */
    template<typename A>
    static std::false_type test(...) {
      return std::false_type(); 
    }

    /* This will be either `std::true_type` or `std::false_type` */
    typedef decltype(test<T>(0,0)) type;

    static const bool value = type::value; /* Which is it? */

    /*  `eval(T const &,std::true_type)` 
	delegates to `T::foo()` when `type` == `std::true_type`
    */
    static void eval(T& t, T* t2, std::true_type) {
      t.onMove(t2);
    }
    /* `eval(...)` is a no-op for otherwise unmatched arguments */
    template <typename... Args>
    static void eval(Args...) {}

    /* `eval(T const & t)` delegates to :-
       - `eval(t,type()` when `type` == `std::true_type`
       - `eval(...)` otherwise
    */  
    static void eval(T& t, T* t2) {
      eval(t,t2,type());
    }
  };

  template <typename T, typename Alloc=GameAllocator>
    struct Array {

    typedef u32 SizeType;

    struct Iterator {
      T* ptr;
      Iterator(T* _ptr) : ptr(_ptr) {}
      void operator++() {
	ptr+=sizeof(T);
      }
      T& operator*() const {
	return *ptr;
      }
      bool operator==(Iterator b) const {
	return ptr == b.ptr;
      }
      bool operator!=(Iterator b) const {
	return ptr != b.ptr;
      }
    };

    void* elements;
    SizeType tot_length;
    SizeType used_length;

    Array(SizeType _length=2)
      : elements(Alloc::malloc(_length*sizeof(T))),
	tot_length(_length),
	used_length(0) {

    }

    Array(InitList<T> list)
      : elements(Alloc::malloc(list.size()*sizeof(T))),
	tot_length(list.size()),
	used_length(0) {
      for (const T& elem : list) {
	emplace_back(elem);
      }
    }

    Array(Array<T>& arr)
      : elements(Alloc::malloc(arr.tot_length*sizeof(T))),
	tot_length(arr.tot_length),
	used_length(arr.used_length) {
      memcpy(elements, arr.elements, tot_length*sizeof(T));
    }

    Array(Array<T>&& arr)
      : elements(arr.elements),
	tot_length(arr.tot_length),
	used_length(arr.used_length) {
      arr.elements = NULL;
    }
    
    ~Array() {
      clear();
      Alloc::free(elements);
    }

    Iterator begin() {
      return (T*) elements;
    }

    Iterator end() {
      return (T*) elements + used_length*sizeof(T);
    }

    SizeType size() {
      return used_length;
    }

    void reserve(SizeType size) {
      void* new_elements = Alloc::malloc(size*sizeof(T));
      for (SizeType i=0; i<used_length*sizeof(T); i+=sizeof(T)) {
	T& item = *((T*) elements+i);
	new ((T*) new_elements+i) T(std::move(item));
	item.~T();
      }
      tot_length = size;
      free(elements);
      elements = new_elements;
      /*elements = Alloc::realloc(elements, tot_length*sizeof(T));
      for (T& elem : *this) {
	HasOnMove<T>::eval(elem);
	}*/
    }

    T* reserveElem() {
      if (used_length >= tot_length) {
	reserve(2*tot_length);
      }
      return (T*) elements + (used_length++)*sizeof(T);
    }

    void push_back(T& elem) {
      new(reserveElem()) T(elem);
    }
  
    template <typename... Args>
    void emplace_back(Args... args) {
      new(reserveElem()) T(args...);
    }

    void pop_back() {
      --used_length;
      (*end()).~T();
    }

    void clear() {
      used_length = 0;
      for (T& elem : *this) {
	elem.~T();
      }
    }

    T& operator[](SizeType index) {
      debugAssert(index < used_length,
		  "The Array index is"
		  " out of bounds");
      return *((T*) elements + index*sizeof(T));
    }

    Array<T>& operator=(const Array<T>& arr) {
      Array tmp(arr);
      *this = std::move(tmp);
      return *this;
    }

    Array<T>& operator=(Array<T>&& arr) {
      *this = arr;
      arr.elements = NULL;
    }
    
  };

}

