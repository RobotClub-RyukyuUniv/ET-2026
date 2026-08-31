#include "app.h"
#include <stdio.h>

#include <libcpp/spike/Motor.h> 
#include <libcpp/spike/Clock.h>
#include <libcpp/spike/ForceSensor.h>
#include <libcpp/spike/IMU.h>

using namespace spikeapi; 

/**
 * メインタスク
 */
void main_task(intptr_t unused) { 

  Motor leftWheel(EPort::PORT_B,Motor::EDirection::COUNTERCLOCKWISE,true); // <1>
  Motor rightWheel(EPort::PORT_A,Motor::EDirection::CLOCKWISE,true);  // <2>
  ForceSensor forceSensor(EPort::PORT_D); // <3>
  Clock clock; // <4>
  IMU imu;

  const int moving_speed = 270;
  const int rotating_speed = 90;
  const uint32_t moving_duration = 2000 * 1000; // 2秒前進
  const uint32_t loop_interval = 50;     // 50μ秒

  while(1) {
    // 1. 前進する
    printf("moving...\n");
    leftWheel.setSpeed(moving_speed);
    rightWheel.setSpeed(moving_speed);
    clock.sleep(moving_duration);

    // 2. 右を見る (+90度まで)
    printf("looking right (+90 deg)...\n");
    imu.resetHeading(); 
    while(1) {
      float current_angle = imu.getHeading();
      if (current_angle >= 90.0f) {
        break;
      }
      leftWheel.setSpeed(rotating_speed);
      rightWheel.setSpeed(-rotating_speed);
      clock.sleep(loop_interval);
    }

    // 3. 左を見る (-180度まで)
    printf("looking left (-180 deg)...\n");
    imu.resetHeading(); 
    while(1) {
      float current_angle = imu.getHeading();
      if (current_angle <= -180.0f) { // マイナス方向へ180度
        break;
      }
      leftWheel.setSpeed(-rotating_speed);
      rightWheel.setSpeed(rotating_speed);
      clock.sleep(loop_interval);
    }

    // 4. 正面を見る (0度に戻す、または必要に応じた角度)
    printf("looking straight...\n");
    imu.resetHeading(); 
    while(1) {
      float current_angle = imu.getHeading();
      if (current_angle >= 90.0f) {
        break;
      }
      leftWheel.setSpeed(rotating_speed);
      rightWheel.setSpeed(-rotating_speed);
      clock.sleep(loop_interval);
    }

    // 少し一時停止
    leftWheel.stop();
    rightWheel.stop();

    clock.sleep(1000); // 1秒停止して次の周回へ
  } 

  printf("Stopped.\n");
  leftWheel.stop();
  rightWheel.stop();
  // フォースセンサーの押された状態が解除されたかを調べる
  while(forceSensor.isTouched()) {
      clock.sleep(50);
  }

  ext_tsk(); 
}
