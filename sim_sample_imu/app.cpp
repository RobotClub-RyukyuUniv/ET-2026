#include "app.h"
#include <stdio.h>

#include "Motor.h"
#include "Clock.h"
#include "IMU.h"

using namespace spikeapi; 

/**
 * メインタスク
 */
void main_task(intptr_t unused) {

  Motor leftWheel(EPort::PORT_B, Motor::EDirection::COUNTERCLOCKWISE, true);
  Motor rightWheel(EPort::PORT_A, Motor::EDirection::CLOCKWISE, true);
  Clock clock;
  IMU imu;

  IMU::Acceleration accel;
  IMU::AngularVelocity ang_vel;
  float azimuth; // 方位角という意味らしい

  imu.resetHeading();

  const int8_t pwm = 30;
  const int32_t duration = 2000 * 1000;

  int reset_counter = 0;

  // 起動時のヘッダー表示
  printf("=== IMU Logger Started ===\n");

  while(1) {

    leftWheel.setPower(pwm);
    rightWheel.setPower(-pwm);

    imu.getAcceleration(accel);      // 加速度を入手する
    imu.getAngularVelocity(ang_vel); // 角速度を入手する
    azimuth = imu.getHeading();      // 方位角を入手する

    // ★ 50ms × 25回 = 1250ms（1.25秒）に1回だけログ出力する
    // 加速度センサーの値が大きいが、単位がmm/s^2となっていて
    // 地球の重力は9.8 m/s^2 = 9,800 mm/s^2 となっていてるから大きくなっている。
    // また本来ならz軸だけ値があるが、IMUは傾いているからx, y, z軸どっちもある。(長さを求めたらちゃんと約9,800となる)
    // なんかシミュレータ上では加速度センサーが0付近になる、、、あと方位角も反応なし、、、
    printf("ACC[%.2f, %.2f, %.2f] | GYRO[%.2f, %.2f, %.2f] | AZI: %.1f\n",
            accel.x, accel.y, accel.z,
            ang_vel.x , ang_vel.y, ang_vel.z,
            azimuth);

    // ★ 50ms × 10回 = 500ms（0.50秒）に1回だけログ出力する
    if (++reset_counter >= 10) {
      imu.resetHeading();
      
      printf("\n");
      printf("Rest!\n");
      printf("\n");

      reset_counter = 0;
    }

  clock.sleep(50);
  } 
}
