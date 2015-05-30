#include "Logger.h"

NAMESPACE {
  namespace log {
    
    static FILE* logfile;
    static bool print_messages;
    const static char* ERROR_MSG = "\033[1m\x1B[31mError!\033[0m ";
  
    void init(const char* filename, bool print_log) {

      if (filename != NULL) {
	logfile = fopen(filename, "w");
	if (logfile == NULL) {
	  fprintf(stderr, "%sCouldn't open log file\n", ERROR_MSG);
	  exit(EXIT_FAILURE);
	}
	print_messages = print_log;
      } else {
	logfile = stdout;
	print_messages = false;
      }
    }

    void vMessage(const char* message, va_list argptr) {
      
      vfprintf(logfile, message,  argptr);
      fprintf(logfile, "\n");
      fflush(logfile);
    
      if (print_messages) {
	vprintf(message, argptr);
	printf("\n");
      }	
    }
    
    void vError(const char* message, va_list argptr) {
      
      if (logfile != stdout) {
	fprintf(logfile, "%s", ERROR_MSG);
	vfprintf(logfile, message, argptr);
	fprintf(logfile, "\n");
      }

      fprintf(stderr, "%s", ERROR_MSG);
      vfprintf(stderr, message, argptr);
      fprintf(stderr, "\n");
    }
  
    void message(const char* message, ...) {
      va_list argptr;
      va_start(argptr, message);
      vMessage(message, argptr);
      va_end(argptr);
    }
  
    void error(const char* message, ...) {
      va_list argptr;
      va_start(argptr, message);
      vError(message, argptr);
      va_end(argptr);
    }

    void exitError(const char* message, ...) {
      va_list argptr;
      va_start(argptr, message);
      vError(message, argptr);
      va_end(argptr);
      exit(EXIT_FAILURE);
    }

    void message(String message, ...) {
      va_list argptr;
      va_start(argptr, message);
      vMessage(message.c_str(), argptr);
      va_end(argptr);
    }
  
    void error(String message, ...) {
      va_list argptr;
      va_start(argptr, message);
      vError(message.c_str(), argptr);
      va_end(argptr);
    }

    void exitError(String message, ...) {
      va_list argptr;
      va_start(argptr, message);
      vError(message.c_str(), argptr);
      va_end(argptr);
      exit(EXIT_FAILURE);
    }
    
  }
}
