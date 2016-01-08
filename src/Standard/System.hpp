#pragma once


#include "Macros.hpp"
#include "Containers.hpp"
#include "Thread.hpp"

NAMESPACE {

  struct System {
    virtual void update() = 0;
    //virtual ~System();
  };

  enum SystemManagerState {
    RUNNING,
    PAUSED,
    KILLED
  };

  struct SystemManager {

    Thread thread;
    Array<System*> systems;
    Mutex state_mutex;
    Mutex system_mutex;
    SystemManagerState state;

    SystemManager(Array<System*> managed_systems);
    void addSystem(System* sys);
    void start();
    void pause();
    ~SystemManager();
  };

}
