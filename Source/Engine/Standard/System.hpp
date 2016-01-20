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
    SYSTEM_RUNNING,
    SYSTEM_PAUSED,
    SYSTEM_KILLED
  };

  struct SystemManager {

    Thread thread;
    Mutex state_mutex;
    Mutex system_mutex;
    Array<System*> systems;
    SystemManagerState state;

    SystemManager(Array<System*> managed_systems = {});
    void addSystem(System* sys);
    void start();
    void pause();
    ~SystemManager();
  };

}
