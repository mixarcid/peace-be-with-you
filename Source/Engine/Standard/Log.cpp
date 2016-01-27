#include <cstdarg>
#include "Log.hpp"
#include "Exception.hpp"

NAMESPACE {

  Log Log::logger;
  
  const static char* LOG_MSG = LOG_STYLE_BOLD "[" LOG_COLOR_LOG	\
    "LOG" LOG_MESSAGE_RESET LOG_STYLE_BOLD "]" \
    LOG_MESSAGE_RESET " ";
  const static char* ERROR_MSG = LOG_STYLE_BOLD "[" \
    LOG_COLOR_ERROR "ERROR" LOG_MESSAGE_RESET LOG_STYLE_BOLD \
    "]" LOG_MESSAGE_RESET " ";

  void _assert_print(const char* message, ...) {
    Log::logger.update();
    va_list args;
    va_start(args, message);
    fprintf(stderr, "%s", ERROR_MSG);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    va_end(args);
  }

  void _assert_print(String message) {
    Log::logger.update();
    fprintf(stderr, "%s%s\n", ERROR_MSG, message.c_str());
  }
  
  LogMessage::LogMessage(LogMessageType msg_type, String msg)
    : type(msg_type),
    message(msg) {}

  LogMessage::LogMessage(LogMessage& msg)
    : type(msg.type),
    message(msg.message.c_str()) {}

  LogMessage::LogMessage(LogMessage&& msg)
    : type(msg.type),
    message(msg.message.c_str()) {}

  LogMessage& LogMessage::operator=(LogMessage& msg) {
    type = msg.type;
    message = msg.message.c_str();
    return *this;
  }

  LogMessage& LogMessage::operator=(LogMessage&& msg) {
    type = msg.type;
    message = msg.message.c_str();
    return *this;
  }

  void Log::processEvent(LogMessage msg) {

    const char* c_msg = msg.message.c_str();
    
    switch (msg.type) {
    case MESSAGE:
      fprintf(Log::logger.logfile, "%s%s\n", LOG_MSG, c_msg);
      fflush(Log::logger.logfile);
      if (Log::logger.print_messages) {
	printf("%s%s\n", LOG_MSG, c_msg);
      }	
      break;
    case ERROR:
      fprintf(Log::logger.logfile, "%s%s\n", ERROR_MSG, c_msg);
      if (Log::logger.print_messages) {
	fprintf(stderr, "%s%s\n", ERROR_MSG, c_msg);
      }
      fflush(Log::logger.logfile);
      break;
    case FATAL_ERROR:
      fprintf(Log::logger.logfile, "%s%s\n", ERROR_MSG, c_msg);
      if (Log::logger.print_messages) {
	fprintf(stderr, "%s%s\n", ERROR_MSG, c_msg);
      }
      fflush(Log::logger.logfile);
      throw FatalError(msg.message);
    }
  }
  
  void Log::init(const char* filename, bool print_log) {

    if (filename != NULL) {
      Log::logger.logfile = fopen(filename, "w");
      if (Log::logger.logfile == NULL) {
	fprintf(stderr, "%sCouldn't open log file\n", ERROR_MSG);
	exit(EXIT_FAILURE);
      }
      Log::logger.print_messages = print_log;
    } else {
      Log::logger.logfile = stdout;
      Log::logger.print_messages = false;
    }
  }

  void Log::terminate() {
    Log::logger.update();
    fclose(Log::logger.logfile);
  }

  Log* Log::getSystem() {
    return &Log::logger;
  }

  void Log::message(String message) {
    Log::logger.emplaceEvent(MESSAGE, message);
  }
  
  void Log::error(String message) {
    Log::logger.emplaceEvent(ERROR, message);
  }

  void Log::fatalError(String message) {
    throw FatalError(message);
  }

  void Log::message(const char* message, ...) {
    va_list args;
    va_start(args, message);
    Log::message(str::vformat(message, args));
    va_end(args);
  }

  void Log::error(const char* message, ...) {
    va_list args;
    va_start(args, message);
    Log::error(str::vformat(message, args));
    va_end(args);
  }

  void Log::fatalError(const char* message, ...) {
    va_list args;
    va_start(args, message);
    Log::fatalError(str::vformat(message, args));
    va_end(args);
  }
  
}
