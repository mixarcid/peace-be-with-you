#pragma once

#include "Macros.hpp"
#include "Containers.hpp"
#include "System.hpp"

NAMESPACE {

  template<typename T>
    struct EventManager : System {

    Mutex event_mutex;
    Array<T> events;

    virtual void processEvent(T event) = 0;
    void addEvent(T event) {
      event_mutex.lock();
      events.push_back(event);
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
