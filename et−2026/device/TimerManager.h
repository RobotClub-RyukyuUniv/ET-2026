#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include "Clock.h"

using namespace spikeapi;

class TimerManager {
public:
  TimerManager();
  void sleep(uint64_t duration);
  uint64_t now() const;

private:
  Clock clock;
};

#endif // TIMER_MANAGER_H