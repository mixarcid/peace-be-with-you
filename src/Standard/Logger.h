#pragma once

#include <cstdio>
#include "Macros.h"
#include "Containers.h"

NAMESPACE {
  namespace log {
    
    /*const char* COLOR_ERROR = "\x1B[31m";
    const char* STYLE_BOLD = "\033[1m";
    const char* MESSAGE_RESET = "\033[0m";*/
  
    void init(const char* filename, bool print_log=false);
    
    void vMessage(const char* message, va_list argptr);
    void vError(const char* message, va_list argptr);

    void message(const char* message, ...);
    void error(const char* message, ...);
    void exitError(const char* message, ...);
    
    void message(String message, ...);
    void error(String message, ...);
    void exitError(String message, ...);
  }
}
