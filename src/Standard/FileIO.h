#pragma once

#include <stdio.h>
#include <stdint.h>
#include "Macros.h"
#include "String.h"

NAMESPACE {
  namespace fio {
    
    struct TestEndian {
      
      bool isLittle;

      TestEndian() {
	int t = 1;
	isLittle = true;
	isLittle = (*((char*)&t) == 1);
      }
      
    };

    const TestEndian ENDIAN;

    template <typename T>
    T readLittleEndian(FILE* file) {

      T data;
      fread(&data, sizeof(T), 1, file);
      
      if (!ENDIAN.isLittle) {
	//Thanks, Michael Manner from Stack Overflow
	unsigned char* lo = (unsigned char*) &data;
	unsigned char* hi = (unsigned char*) &data + sizeof(T) - 1;
	unsigned char swap;
	while (lo < hi) {
	  swap = *lo;
	  *lo++ = *hi;
	  *hi-- = swap;
	  }
      }

      return data;
    }
    
    String readString(FILE* file);
    
  }
}