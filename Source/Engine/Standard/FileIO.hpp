#pragma once

#include <stdio.h>
#include <stdint.h>
#include "Macros.hpp"
#include "Log.hpp"
#include "String.hpp"
#include "Exception.hpp"

NAMESPACE {
  namespace fio {
    
    struct TestEndian {
      
      bool isLittle;

      TestEndian() : isLittle(true) {
	int t = 1;
	isLittle = (*((char*)&t) == 1);
      }
      
    };

    const TestEndian ENDIAN;

    template <typename T>
    T byteReverse(T val) {
      //Thanks, Michael Manner from Stack Overflow
      T data = val;
      unsigned char* lo = (unsigned char*) &data;
      unsigned char* hi = (unsigned char*) &data + sizeof(T) - 1;
      unsigned char swap;
      while (lo < hi) {
	swap = *lo;
	*lo++ = *hi;
	*hi-- = swap;
      }
      return data;
    }
    
    template <typename T>
    T readLittleEndian(FILE* file) {

      T data;
      fatalAssert(fread(&data, sizeof(T), 1, file) != 0,
		  "Unexpected EOF while reading. Or possibly"
		  " another file I/O error");
      
      if (!ENDIAN.isLittle) {
	data = fio::byteReverse(data);
      }

      return data;
    }
    
    String readString(FILE* file);
    
  }
}