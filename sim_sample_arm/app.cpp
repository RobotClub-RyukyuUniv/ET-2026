#include "app.h"
#include <stdio.h>

#include "Motor.h" 
#include "Clock.h" 
#include "ForceSensor.h" 

using namespace spikeapi; 

/**
 * メインタスク
 */
void main_task(intptr_t unused) {

  Motor armMotor(EPort::PORT_C, Motor::EDirection::COUNTERCLOCKWISE, true); // <1>
  ForceSensor forceSensor(EPort::PORT_D); // <3>
  Clock clock; // <4>

  const int8_t pwm = 15;

  const uint32_t duration = 2000*1000;

  while(1) { 
    printf("Forwarding...\n");
    armMotor.setPower(pwm);
    clock.sleep(duration);

    armMotor.setPower(-pwm);
    clock.sleep(duration);

    // フォースセンサーが押されているかどうか調べる
    if (forceSensor.isTouched()) {
      break;
    }
  } 

  printf("Stopped.\n");
  armMotor.stop();
  // フォースセンサーの押された状態が解除されたかを調べる
  while(forceSensor.isTouched()) {
      ;
  }

  ext_tsk(); 
}
