#pragma once

#include "Macros.hpp"
#include "System.hpp"

NAMESPACE {

  template<typename T>
    struct EventManager : System {

    Mutex event_mutex;
    Array<T> events;

    virtual void processEvent(T event) = 0;
    void addEvent(T event) {
      events.push_back(event);
    }
    
    template <typename... Args>
    void emplaceEvent(Args... args) {
      event_mutex.lock();
      events.emplace_back(args...);
      event_mutex.unlock();
    }
    
    virtual void update() {
      event_mutex.lock();
      for (T event : events) {
	processEvent(event);
      }
      events.clear();
      event_mutex.unlock();
    }
  };

}
