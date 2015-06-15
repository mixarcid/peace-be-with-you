#pragma once

#include "Macros.hpp"
#include "String.hpp"
#include "EventManager.hpp"

#define LOG_COLOR_ERROR "\x1B[31m"
#define LOG_COLOR_LOG "\x1B[36m"
#define LOG_STYLE_BOLD "\033[1m"
//#define LOG_COLOR_DEFAULT "\x1B[49m"
#define LOG_MESSAGE_RESET "\033[0m"

NAMESPACE {

  enum LogMessageType {
    MESSAGE,
    ERROR,
    FATAL_ERROR
  };

  struct LogMessage {

    LogMessageType type;
    String message;

    LogMessage(LogMessageType msg_type, String msg);
    
  };

  struct Log : EventManager<LogMessage> {

    FILE* logfile;
    bool print_messages;

    virtual void processEvent(LogMessage msg);
    //using EventManager<LogMessage>::update();
  
    static void init(const char* filename, bool print_Log=false);
    static void terminate();

    static void message(const char* message, ...);
    static void error(const char* message, ...);
    static void fatalError(const char* message, ...);
    
    static void message(String message);
    static void error(String message);
    static void fatalError(String message);

    static Log logger;
    
  };
  
}
