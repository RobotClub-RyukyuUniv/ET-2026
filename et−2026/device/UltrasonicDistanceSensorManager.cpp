#include "UltrasonicDistanceSensorManager.h"

UltrasonicDistanceSensorManager::UltrasonicDistanceSensorManager(EPort port) :
  ultrasonicSensor(port) {
}

int32_t UltrasonicDistanceSensorManager::getDistance() const {
  return ultrasonicSensor.getDistance();
}