#include "DualMotorController.h"

DualMotorController::DualMotorController():
  leftWheel(EPort::PORT_B, Motor::EDirection::COUNTERCLOCKWISE, true),
  rightWheel(EPort::PORT_A, Motor::EDirection::CLOCKWISE, true) {
}

void DualMotorController::setSpeed(int leftSpeed, int rightSpeed) {
  leftWheel.setSpeed(leftSpeed);
  rightWheel.setSpeed(rightSpeed);
}

void DualMotorController::setPwm(int leftPwm, int rightPwm) {
  leftWheel.setPower(leftPwm);
  rightWheel.setPower(rightPwm);
}

void DualMotorController::stop() {
  leftWheel.stop();
  rightWheel.stop();
}

int32_t DualMotorController::getCountLeft() {
  return leftWheel.getCount();
}

int32_t DualMotorController::getCountRight() {
  return rightWheel.getCount();
}

void DualMotorController::resetCount() {
  leftWheel.resetCount();
  rightWheel.resetCount();
}