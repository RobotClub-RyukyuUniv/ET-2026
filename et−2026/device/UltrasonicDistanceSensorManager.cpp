#include "UltrasonicDistanceSensorManager.h"

UltrasonicDistanceSensorManager::UltrasonicDistanceSensorManager() :
  ultrasonicSensor(EPort::PORT_F) {
}

int32_t UltrasonicDistanceSensorManager::getDistance() const {
  return ultrasonicSensor.getDistance();
}

bool UltrasonicDistanceSensorManager::isPresence() const {
  return ultrasonicSensor.isPresence();
}