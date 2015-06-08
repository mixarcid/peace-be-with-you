#pragma once

#include "Macros.h"
#include "String.h"

#define LOG_COLOR_ERROR "\x1B[31m"
#define LOG_COLOR_LOG "\x1B[36m"
#define LOG_STYLE_BOLD "\033[1m"
//#define LOG_COLOR_DEFAULT "\x1B[49m"
#define LOG_MESSAGE_RESET "\033[0m"

NAMESPACE {
  namespace log {

    const unsigned int MAX_FATAL_MESSAGE_LENGTH = 200;
  
    void init(const char* filename, bool print_log=false);
    void terminate();
    
    void vMessage(const char* message, va_list argptr);
    void vError(const char* message, va_list argptr);

    void message(const char* message, ...);
    void error(const char* message, ...);
    void exitError(const char* message, ...);
    void fatalError(const char* message, ...);
    
    void message(String message, ...);
    void error(String message, ...);
    void exitError(String message, ...);
    void fatalError(String message, ...);
    
  }
}
