#ifndef ULTRASONIC_DISTANCE_SENSOR_MANAGER_H
#define ULTRASONIC_DISTANCE_SENSOR_MANAGER_H

#include "UltrasonicSensor.h"

using namespace spikeapi;

class UltrasonicDistanceSensorManager {
public:
  UltrasonicDistanceSensorManager();
  int32_t getDistance() const;
  bool isPresence() const;

private:
  spikeapi::UltrasonicSensor ultrasonicSensor;
};

#endif // ULTRASONIC_DISTANCE_SENSOR_MANAGER_H