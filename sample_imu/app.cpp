#include "app.h"
#include <stdio.h>
#include <serial/newlib.h>
#include <serial/serial.h>

#include <libcpp/spike/Clock.h>
#include <libcpp/spike/IMU.h>

using namespace spikeapi; 

/**
 * メインタスク
 */
void main_task(intptr_t unused) {
  // USBシリアルポートを標準出力(printf)に割り当てる
  stdout = serial_open_newlib_file(SIO_USB_PORTID);

  Clock clock;
  IMU imu;

  IMU::Acceleration accel;
  IMU::AngularVelocity ang_vel;
  float azimuth; // 方位角という意味らしい

  imu.resetHeading();

  int log_counter = 0;
  int reset_counter = 0;

  // 起動時のヘッダー表示
  printf("=== IMU Logger Started ===\n");

  while(1) { 
    imu.getAcceleration(accel);      // 加速度を入手する
    imu.getAngularVelocity(ang_vel); // 角速度を入手する
    azimuth = imu.getHeading();      // 方位角を入手する

    // ★ 50ms × 25回 = 1250ms（1.25秒）に1回だけログ出力する
    // 加速度センサーの値が大きいが、単位がmm/s^2となっていて
    // 地球の重力は9.8 m/s^2 = 9,800 mm/s^2 となっていてるから大きくなっている。
    // また本来ならz軸だけ値があるが、IMUは傾いているからx, y, z軸どっちもある。(長さを求めたらちゃんと約9,800となる)
    if (++log_counter >= 25) {
      printf("ACC[%.2f, %.2f, %.2f] | GYRO[%.2f, %.2f, %.2f] | AZI: %.1f\n",
             accel.x, accel.y, accel.z,
             ang_vel.x , ang_vel.y, ang_vel.z,
             azimuth);

      log_counter = 0;
    }

    // ★ 50ms × 100回 = 5000ms（5.00秒）に1回だけログ出力する
    if (++reset_counter >= 1000) {
      imu.resetHeading();
      
      printf("\n");
      printf("Rest!\n");
      printf("\n");

      reset_counter = 0;
    }

  clock.sleep(50);
  } 
}
