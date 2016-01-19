#include "FileIO.hpp"

NAMESPACE {
  namespace fio {

    String readString(FILE* file) {
      String ret;
      char c = (char) getc(file);
      while (c != '\0') {
	ret += c;
	c = (char) getc(file);
      }
      return ret;
    }
    
  }
}
