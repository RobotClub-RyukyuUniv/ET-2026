#include "Motor.h"       // <1>
#include "ColorSensor.h" // <2>

using namespace spikeapi; // <3>

class Tracer {  // <1>
public:
  Tracer();
  void run();       // <2>
  void init();
  void terminate();

private:
  Motor leftWheel;
  Motor rightWheel;
  ColorSensor colorSensor; // <3>

  const int8_t mThreshold = 20;  // <4>
  int integral = 0;              // 誤差の累積（積分用）
  int prev_diff = 0;             // 前回の誤差（微分用）
#ifndef MAKE_RASPIKE
  const int8_t pwm = 40;
#else
  const int8_t pwm = 38;
#endif
};
