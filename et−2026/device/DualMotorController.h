#include "Motor.h"

using namespace spikeapi;

class LeftRightMotor {
public:
  LeftRightMotor();
  void setSpeed(int leftSpeed, int rightSpeed);
  void stop();
  int32_t getCountLeft();
  int32_t getCountRight();
  void resetCount();

private:
  Motor leftWheel;
  Motor rightWheel;
};