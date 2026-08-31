#include "app.h"
#include <stdio.h>
#include <serial/newlib.h>
#include <serial/serial.h>

#include <libcpp/spike/Motor.h> 
#include <libcpp/spike/Clock.h>
#include <libcpp/spike/ForceSensor.h>
#include <libcpp/spike/IMU.h>

using namespace spikeapi; 

/**
 * メインタスク
 */
void main_task(intptr_t unused) {
  // USBシリアルポートを標準出力(printf)に割り当てる
  stdout = serial_open_newlib_file(SIO_USB_PORTID);

  Motor leftWheel(EPort::PORT_B,Motor::EDirection::COUNTERCLOCKWISE,true); // <1>
  Motor rightWheel(EPort::PORT_A,Motor::EDirection::CLOCKWISE,true);  // <2>
  ForceSensor forceSensor(EPort::PORT_D); // <3>
  Clock clock; // <4>
  IMU imu;

  const int8_t speed = 10;

  const uint32_t duration = 50;

  imu.resetHeading(); // 方位の初期化
  const float target_angle = 45;

  int log_counter = 0;
  bool is_rotating;

  printf("rotate to %f degree...\n", target_angle);

  while(1) { 

    float current_angle = imu.getHeading();
    is_rotating = current_angle >= target_angle;

    if (is_rotating) {
      break;
    }

    leftWheel.setSpeed(speed);
    rightWheel.setSpeed(-speed);

    // ★ 50ms × 10回 = 10ms（0.5秒）に1回だけログ出力する
    if (++log_counter >= 10) {
      printf("current_angle : %f, rotate to taget: %s\n", current_angle, is_rotating ? "True" : "False");

      log_counter = 0;
    }

    clock.sleep(duration);
  } 

  printf("Stopped.\n");
  leftWheel.stop();
  rightWheel.stop();
  // フォースセンサーの押された状態が解除されたかを調べる
  while(forceSensor.isTouched()) {
      ;
  }

  ext_tsk(); 
}
