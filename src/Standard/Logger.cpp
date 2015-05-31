#include "Logger.h"
#include "Exceptions.h"

NAMESPACE {
  namespace log {
    
    static FILE* logfile;
    static bool print_messages;
    const static char* LOG_MSG = LOG_STYLE_BOLD "[" LOG_COLOR_LOG	\
      "LOG" LOG_MESSAGE_RESET LOG_STYLE_BOLD "]" LOG_MESSAGE_RESET " ";
    const static char* ERROR_MSG = LOG_STYLE_BOLD "[" LOG_COLOR_ERROR	\
      "ERROR" LOG_MESSAGE_RESET LOG_STYLE_BOLD "]" LOG_MESSAGE_RESET " ";
  
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

      fprintf(logfile, "%s", LOG_MSG);
      vfprintf(logfile, message,  argptr);
      fprintf(logfile, "\n");
      fflush(logfile);
    
      if (print_messages) {
	printf("%s", LOG_MSG);
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

    void fatalError(const char* message, ...) {
      char msg[MAX_FATAL_MESSAGE_LENGTH];
      va_list argptr;
      va_start(argptr, message);
      vsprintf(msg, message, argptr);
      vError(message, argptr);
      va_end(argptr);
      throw FatalError(String(msg));
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

    void fatalError(String message, ...) {
      char msg[MAX_FATAL_MESSAGE_LENGTH];
      va_list argptr;
      va_start(argptr, message);
      vsprintf(msg, message.c_str(), argptr);
      vError(message.c_str(), argptr);
      va_end(argptr);
      throw FatalError(String(msg));
    }
    
  }
}
