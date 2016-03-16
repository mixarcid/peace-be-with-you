#pragma once

#include <unordered_map>
#include <unordered_set>
#include <initializer_list>
#include <string.h>
#include "Types.hpp"
#include "Function.hpp"
#include "GameAllocator.hpp"
#include "Exception.hpp"
#include "Assert.hpp"

NAMESPACE {

  template <typename Key>
    size_t customHash(Key k);
  
}

namespace std {
  template <typename Key>
  struct hash {
    size_t operator()(const Key& k) const {
      return peace::customHash(k);
    }
  };
}

NAMESPACE {

  template <typename Key, typename Val>
    using HashMap = typename std::unordered_map<Key, Val>;
  template <typename Key>
    using Hash = std::hash<Key>;
  template <typename A, typename B>
    using Pair = typename std::pair<A, B>;
  template <typename T>
    using UnorderedSet = std::unordered_set<T>;
  template <typename T>
    using InitList = typename std::initializer_list<T>;

  template <typename A, typename B>
    Pair<A,B> makePair(A a, B b) {
    return (Pair<A,B>(a,b));
  }

  //Thanks, yrp from StackOverflow!
  template <typename T>
    struct HasOnMove {

    /* SFINAE foo-has-correct-sig :) */

    template<typename A>
    static std::true_type test(void(A::*)()) {
      return std::true_type();
    }

    /* SFINAE foo-exists :) */
    template <typename A> 
    static decltype(test(&A::_on_move)) 
    test(decltype(&A::_on_move),void *) {
      /* foo exists. What about sig? */
      typedef decltype(test(&A::_on_move)) return_type; 
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
    static void eval(T& t, std::true_type) {
      t._on_move();
    }

    /* `eval(...)` is a no-op for otherwise unmatched arguments */
    template <typename... Args>
    static void eval(Args...) {}

    /* `eval(T const & t)` delegates to :-
       `eval(t,type()` when `type` == `std::true_type`
       `eval(...)` otherwise
    */  
    static void eval(T& t) {
      eval(t,type());
    }

  };

  //to be called when a specific type is moved to the new position ptr
  template<typename T>
    void _on_move(T* ptr) {
    HasOnMove<T>::eval(*ptr);
  }

  template <typename T, typename Alloc=GameAllocator>
    struct Array {

    typedef u32 SizeType;
    typedef T* Iterator;

    T*  elements;
    SizeType tot_length;
    SizeType used_length;

    Array(SizeType _length=8)
      : elements((T*)Alloc::malloc(_length*sizeof(T))),
	tot_length(_length),
	used_length(0) {
    }

    Array(InitList<T> list)
      : elements((T*)Alloc::malloc(list.size()*sizeof(T))),
	tot_length(list.size()),
	used_length(0) {
      for (const T& elem : list) {
	emplace_back(elem);
      }
    }

    Array(const Array& arr)
      : elements((T*)Alloc::malloc(arr.tot_length*sizeof(T))),
	tot_length(arr.tot_length),
	used_length(0) {
      for (T& elem : arr) {
	push_back(elem);
      }
    }

    Array(Array&& arr)
      : elements(arr.elements),
	tot_length(arr.tot_length),
	used_length(arr.used_length) {
      arr.elements = NULL;
      arr.used_length = 0;
      arr.tot_length = 0;
    }
    
    ~Array() {
      clear();
      if (elements) {
	Alloc::free(elements);
      }
    }

    Iterator begin() const {
      return elements;
    }

    Iterator end() const {
      return elements + used_length;
    }

    T& front() const {
      return *begin();
    }

    T& back() const {
      return *(end()-1);
    }

    SizeType size() const {
      return used_length;
    }

    void reserve(SizeType new_size) {
      debugAssert(new_size >= used_length,
		  "Array::reserve called with new size less than old size");
      tot_length = new_size;
      T* old_elements = elements;
      elements = (T*)Alloc::realloc(elements, tot_length*sizeof(T));
      if (old_elements != elements) {
	for (T& elem : *this) {
	  _on_move(&elem);
	}
      }
    }

    T* reserveElem() {
      if (used_length >= tot_length) {
	reserve(2*tot_length);
      }
      SizeType offset = (used_length++);
      T* ret = elements + offset;
      return ret;
    }

    Iterator push_back(T const & elem) {
      return new(reserveElem()) T(elem);
    }
  
    template <typename U=T, typename... Args>
    typename Array<U>::Iterator emplace_back(Args... args) {
      debugAssert(sizeof(U) == sizeof(T),
		  "Array::emplace_back only accepts"
		  " types that have the same size as"
		  " the Array type");
      return new(reserveElem()) U(args...);
    }

    void pop_back() {
      back().~T();
      --used_length;
    }

    void clear() {
      for (T& elem : *this) {
	elem.~T();
      }
      used_length = 0;
    }

    void removeAndReplace(Iterator it) {
      debugAssert(it >= begin() && it < end(),
		  "Invalid Iterator passed to "
		  "Array::insert");
      if (used_length > 1) {
	it->~T();
	if (it != end()-1) {
	  memcpy((void*)it,(void*)(end()-1),sizeof(T));
	  _on_move(it);
	}
	--used_length;
      } else {
	clear();
      }
    }

    Iterator insert(Iterator pos, T& item) {
      if (pos == end() - 1) {
	push_back(item);
      }
      debugAssert(pos >= begin() && pos < end(),
		  "Invalid Iterator passed to "
		  "Array::insert");
      if (used_length >= tot_length) {
	reserve(2*tot_length);
      }
      ++used_length;
      memmove((void*)(pos+1),
	      (void*)pos,
	      (SizeType)((char*)end()-(char*)pos));
      for (Iterator it=pos+1; it<end(); ++it) {
	_on_move(it);
      }
      return new(pos) T(item);
    }

    template <typename U=T, typename... Args>
    typename Array<U>::Iterator emplace(Iterator pos, Args... args) {
      debugAssert(sizeof(U) == sizeof(T),
		  "Array::emplace only accepts"
		  " types that have the same size as"
		  " the Array type");
      debugAssert(pos >= begin() && pos < end(),
		  "Invalid Iterator passed to "
		  "Array::emplace");
      if (used_length >= tot_length) {
	reserve(2*tot_length);
      }
      ++used_length;
      memmove((void*)(pos+1),
	      (void*)pos,
	      (SizeType)((char*)end()-(char*)pos));
      for (Iterator it=pos+1; it<end(); ++it) {
	_on_move(it);
      }
      return new(pos) U(args...);
    }

    /*
      crude implimentation, but it ain't bad for
      small arrays

      cmp returns true if we want the second element to
      go behind the first, false otherwise
    */
    Iterator insertSorted(T item,
			  function<bool(T&,T&)> cmp) {
      for (Iterator it = begin(); it != end(); ++it) {
	if (cmp(*it, item)) {
	  return insert(it,item);
	}
      }
      return push_back(item);
    }

    T& operator[](SizeType index) const {
      debugAssert(index < used_length,
		  "The Array index is"
		  " out of bounds");
      return *((T*) elements + index);
    }

    Array& operator=(const Array& arr) {
      Array tmp(arr);
      *this = std::move(tmp);
      return *this;
    }

    Array& operator=(Array&& arr) {
      clear();
      if (elements) {
	Alloc::free(elements);
      }
      elements = arr.elements;
      arr.elements = NULL;
      used_length = arr.used_length;
      arr.used_length = 0;
      tot_length = arr.tot_length;
      arr.tot_length = 0;
      return *this;
    }
  };

  
}

