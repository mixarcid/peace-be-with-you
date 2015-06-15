#include "System.hpp"

NAMESPACE {

  SystemManager::SystemManager(Array<System*> managed_systems) {
    state = PAUSED;
    systems = managed_systems;
    thread = Thread([this]() {
	bool running = true;
	while(running) {
	  state_mutex.lock();
	  SystemManagerState s = state;
	  state_mutex.unlock();
	  switch (s) {
	  case PAUSED:
	    break;
	  case KILLED:
	    //notice the lack of break ;)
	    running = false;
	  case RUNNING:
	    for (System* sys : systems) {
	      sys->update();
	    }
	    break;
	  }
	}
      });
  }

  void SystemManager::start() {
    state_mutex.lock();
    state = RUNNING;
    state_mutex.unlock();
  }

  void SystemManager::pause() {
    state_mutex.lock();
    state = PAUSED;
    state_mutex.unlock();
  }

  SystemManager::~SystemManager() {
    state_mutex.lock();
    state = KILLED;
    state_mutex.unlock();
    thread.join();
  }
}
