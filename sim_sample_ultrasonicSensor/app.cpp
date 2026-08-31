#include "app.h"
#include <stdio.h>

#include "Clock.h" 
#include "UltrasonicSensor.h"

using namespace spikeapi; 

/**
 * メインタスク
 */
void main_task(intptr_t unused) {

  UltrasonicSensor ultrasonicSensor(EPort::PORT_F);
  Clock clock;

  const uint32_t duration = 100 * 1000; // 100m秒

  // 閾値
  const int32_t threshold = 400;

  while (1) {
    int32_t distance = ultrasonicSensor.getDistance();
    bool isReached = distance < threshold;
    bool isPresence = ultrasonicSensor.isPresence();

    if (distance == -1) { // 測定不能の場合-1が返される
      isReached = false;
    }

    printf("distance = %d, is_reached : %s, ispresence : %s\n", 
           distance, 
           isReached ? "true" : "false", 
           isPresence ? "true" : "false");

    clock.sleep(duration);
  }

  ext_tsk(); 
}