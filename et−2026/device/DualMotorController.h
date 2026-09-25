#ifndef DUAL_MOTOR_CONTROLLER_H
#define DUAL_MOTOR_CONTROLLER_H

#include "Motor.h"

using namespace spikeapi;

class DualMotorController {
public:
  DualMotorController();
  void setSpeed(int leftSpeed, int rightSpeed);
  void setPwm(int leftPwm, int rightPwm); // 追加: PWM（パワー）で指定するメソッド
  void stop();
  int32_t getCountLeft();
  int32_t getCountRight();
  void resetCount();

private:
  Motor leftWheel;
  Motor rightWheel;
};

#endif // DUAL_MOTOR_CONTROLLER_H