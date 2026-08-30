#include "app.h"
#include <stdio.h>

#include <libcpp/spike/Motor.h> 
#include <libcpp/spike/Clock.h>
#include <libcpp/spike/ForceSensor.h> 

using namespace spikeapi; 

/**
 * メインタスク
 */
void main_task(intptr_t unused) { 

  Motor armMotor(EPort::PORT_C,Motor::EDirection::CLOCKWISE,true);  // アームの場合はポート番号に注意
  ForceSensor forceSensor(EPort::PORT_D);
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
