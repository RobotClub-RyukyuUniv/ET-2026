#include "app.h"
#include <stdio.h>
#include <serial/newlib.h>
#include <serial/serial.h>

#include <libcpp/spike/Motor.h> 
#include <libcpp/spike/Clock.h>
#include <libcpp/spike/ForceSensor.h>
#include <libcpp/spike/IMU.h>

#include <cmath>

using namespace spikeapi; 

void main_task(intptr_t unused) {
  stdout = serial_open_newlib_file(SIO_USB_PORTID);
  Motor leftWheel(EPort::PORT_B, Motor::EDirection::COUNTERCLOCKWISE, true);
  Motor rightWheel(EPort::PORT_A, Motor::EDirection::CLOCKWISE, true);
  ForceSensor forceSensor(EPort::PORT_D);
  Clock clock;
  IMU imu;

  leftWheel.resetCount();
  rightWheel.resetCount();

  const int8_t speed = 90;
  const int8_t rotating_speed = 90;

  const uint32_t duration = 1500 * 1000;
  const uint32_t loop_interval = 50;

  float delt_x = 0.0;
  float delt_y = 0.0;

  const float pi = 3.14159265;

  // 変数はループの外または最初で宣言する
  float angle = 0;
  float total_angle = 0;
  int32_t left_count = 0;
  int32_t right_count = 0;
  float count = 0.0;

  printf("Start\n");
  while(1) { 
    imu.resetHeading();
    while(1) {
      float current_angle = imu.getHeading();
      if (current_angle >= 60.0f) {
        break;
      }
      leftWheel.setSpeed(rotating_speed);
      rightWheel.setSpeed(-rotating_speed);
      clock.sleep(loop_interval);
    }

    leftWheel.stop();
    rightWheel.stop();

    angle = imu.getHeading();
    total_angle += angle;

    leftWheel.resetCount();
    rightWheel.resetCount();

    leftWheel.setSpeed(speed);
    rightWheel.setSpeed(speed);
    clock.sleep(duration);

    left_count = leftWheel.getCount();
    right_count = rightWheel.getCount();
    count = (left_count + right_count) / 2.0;

    delt_x += count * sinf(pi * total_angle / 180.0);
    delt_y += count * cosf(pi * total_angle / 180.0);

    printf("delt_x : %f, delt_y : %f\n", delt_x, delt_y);

    imu.resetHeading();
    while(1) {
      float current_angle = imu.getHeading();
      if (current_angle <= -90.0f) {
        break;
      }
      leftWheel.setSpeed(-rotating_speed);
      rightWheel.setSpeed(rotating_speed);
      clock.sleep(loop_interval);
    }

    leftWheel.stop();
    rightWheel.stop();

    angle = imu.getHeading();
    total_angle += angle;

    leftWheel.resetCount();
    rightWheel.resetCount();

    leftWheel.setSpeed(speed);
    rightWheel.setSpeed(speed);
    clock.sleep(duration*2);

    // 型をつけずに代入する
    left_count = leftWheel.getCount();
    right_count = rightWheel.getCount();
    count = (left_count + right_count) / 2.0;

    delt_x += count * sinf(pi * total_angle / 180.0);
    delt_y += count * cosf(pi * total_angle / 180.0);

    printf("delt_x : %f, delt_y : %f\n", delt_x, delt_y);

    float cartesian_deg = atan2f(delt_y, delt_x) * 180.0 / pi; // 度数法に変換
    float global_angle_deg = 90.0 - cartesian_deg;

    printf("global angle (deg) : %f, local angle (deg) : %f\n", global_angle_deg, total_angle);
    (void)angle; // 未使用警告回避用など

    break;
  } 

  printf("Stopped.\n");
  leftWheel.stop();
  rightWheel.stop();

  while(forceSensor.isTouched()) {
      ;
  }

  ext_tsk(); 
}