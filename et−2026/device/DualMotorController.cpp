#include "DualMotorController.h"

LeftRightMotor::LeftRightMotor():
  leftWheel(EPort::PORT_B,Motor::EDirection::COUNTERCLOCKWISE,true),
  rightWheel(EPort::PORT_A,Motor::EDirection::CLOCKWISE,true) {
}

void LeftRightMotor::setSpeed(int leftSpeed, int rightSpeed) {
  leftWheel.setPower(leftSpeed);
  rightWheel.setPower(rightSpeed);
}

void LeftRightMotor::stop() {
  leftWheel.stop();
  rightWheel.stop();
}

int32_t LeftRightMotor::getCountLeft() {
  return leftWheel.getCount();
}

int32_t LeftRightMotor::getCountRight() {
  return rightWheel.getCount();
}

void LeftRightMotor::resetCount() {
  leftWheel.resetCount();
  rightWheel.resetCount();
}