#ifndef ULTRASONIC_DISTANCE_SENSOR_MANAGER_H
#define ULTRASONIC_DISTANCE_SENSOR_MANAGER_H

#include <libcpp/spike/UltrasonicSensor.h>

using namespace spikeapi;

class UltrasonicDistanceSensorManager {
public:
  explicit UltrasonicDistanceSensorManager(EPort port);
  int32_t getDistance() const;

private:
  UltrasonicSensor ultrasonicSensor;
};

#endif // ULTRASONIC_DISTANCE_SENSOR_MANAGER_H