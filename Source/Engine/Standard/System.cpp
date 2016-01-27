#include "System.hpp"

NAMESPACE {

  SystemManager::SystemManager(Array<System*> managed_systems)
    : systems(managed_systems),
    state(SYSTEM_PAUSED) {
      
    thread = Thread([this]() {
	bool running = true;
	while(running) {
	  system_mutex.lock();
	  state_mutex.lock();
	  SystemManagerState s = state;
	  state_mutex.unlock();
	  switch (s) {
	  case SYSTEM_PAUSED:
	    break;
	  case SYSTEM_KILLED:
	    //notice the lack of break ;)
	    running = false;
	  case SYSTEM_RUNNING:
	    for (System* sys : systems) {
	      sys->update();
	    }
	    break;
	  }
	  system_mutex.unlock();
	}
      });
  }

  void SystemManager::addSystem(System* sys) {
    system_mutex.lock();
    systems.push_back(sys);
    system_mutex.unlock();
  }

  void SystemManager::start() {
    state_mutex.lock();
    state = SYSTEM_RUNNING;
    state_mutex.unlock();
  }

  void SystemManager::pause() {
    state_mutex.lock();
    state = SYSTEM_PAUSED;
    state_mutex.unlock();
  }

  SystemManager::~SystemManager() {
    state_mutex.lock();
    state = SYSTEM_KILLED;
    state_mutex.unlock();
    thread.join();
    for (System* sys : systems) {
      sys->update();
    }
  }
}
