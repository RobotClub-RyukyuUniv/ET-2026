#include "Tracer.h" // <1>
#include <stdio.h>

Tracer::Tracer():
  leftWheel(EPort::PORT_B,Motor::EDirection::COUNTERCLOCKWISE,true), // <1>
  rightWheel(EPort::PORT_A,Motor::EDirection::CLOCKWISE,true),  // <2>
  colorSensor(EPort::PORT_E) { // <2>
}

void Tracer::init() {
  printf("Tracer\n");
}

void Tracer::terminate() {
  printf("Stopped.\n");
  leftWheel.stop();  // <1>
  rightWheel.stop();
}

void Tracer::run() {
  const float Kp=0.83;	// <1>
  const int target=10;	// <2>
  const int bias=0;		// <3>

  printf("running...\n");
  int diff = colorSensor.getReflection() - target;	// <4>
  float turn = Kp * diff + bias;					// <5>
  leftWheel.setPower(pwm - turn);					// <6>
  rightWheel.setPower(pwm + turn);					// <6>
}
