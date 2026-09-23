#include "TimerManager.h"

TimerManager::TimerManager() : clock() {
}

void TimerManager::sleep(uint64_t duration) {
  clock.sleep(duration);
}

uint64_t TimerManager::now() const {
  return clock.now();
}