#include "FileIO.hpp"

NAMESPACE {

  void load(FILE* file, String* s) {
    char c = (char) getc(file);
    while (c != '\0') {
      *s += c;
      c = (char) getc(file);
    }
  }
  
  void save(FILE* file, String& s) {
    fwrite(s.c_str(), s.length(), 1, file);
  }

}
