#pragma once

#include <sys/timeb.h>
#include "Macros.hpp"

NAMESPACE {

  struct Time {

    struct timeb time;

    void operator=(Time b) {
      time.time = b.time.time;
      time.millitm = b.time.millitm;
    }
    
    void makeCurrent() {
      ftime(&time);
    }

    unsigned long getMilliseconds() {
      return (1000 * time.time) + time.millitm;
    }

  };

}
