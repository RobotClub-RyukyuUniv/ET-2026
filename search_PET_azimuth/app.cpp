#include "app.h"
#include <stdio.h>
#include <serial/newlib.h>
#include <serial/serial.h>

#include <libcpp/spike/Motor.h>
#include <libcpp/spike/ForceSensor.h>
#include <libcpp/spike/UltrasonicSensor.h>
#include <libcpp/spike/IMU.h>
#include <libcpp/spike/Clock.h>

using namespace spikeapi;

void main_task(intptr_t unused) {
  // USBシリアルポートを標準出力(printf)に割り当てる
  stdout = serial_open_newlib_file(SIO_USB_PORTID);

  Motor leftWheel(EPort::PORT_B, Motor::EDirection::COUNTERCLOCKWISE, true);
  Motor rightWheel(EPort::PORT_A, Motor::EDirection::CLOCKWISE, true);
  ForceSensor forceSensor(EPort::PORT_D);
  UltrasonicSensor ultrasonicSensor(EPort::PORT_F);
  IMU imu;
  Clock clock;

  // 速度と距離の設定
  const int moving_speed = 180;      // 直進速度
  const int rotating_speed = 90;     // 旋回速度
  const int target_distance = 400;   // 発見と見なす距離 (400mm以内)
    
  // --- 時間設定（ミリ秒ベース） ---
  const uint32_t sensor_interval_ms = 1000;   // センサー確認の間隔 (ms) = 1.0秒
  const uint32_t log_interval_ms = 100;     // ログ出力の間隔 (ms) = 100m秒
  const uint32_t moving_duration_ms = 2000;  // 直進する時間 (ms) = 2.0秒

  // Clock::sleep や内部計算用にマイクロ秒に変換するマクロ・変数
  auto sleep_ms = [&clock](uint32_t ms) {
      clock.sleep(ms * 1000);
  };

  // ログを出力するまでのカウンタ閾値を自動計算 (例: 100ms / 10ms = 10回に1回)
  const int log_threshold = log_interval_ms / sensor_interval_ms;

  // ボタンを押したら何度も再開できるように全体を無限ループで囲む
  while (true) {
    bool is_discovery = false;
    int log_counter = 0; // ログの間引き用カウンタ

    printf("Start exploring...\n");

    while (!is_discovery) {
      // --- 1. 直進しながら探す ---
      printf("Moving forward...\n");
      leftWheel.setSpeed(moving_speed);
      rightWheel.setSpeed(moving_speed);
      log_counter = 0; // フェーズの最初にリセット

      int steps = moving_duration_ms / sensor_interval_ms;
      for (int i = 0; i < steps; ++i) {
        int32_t distance_mm = ultrasonicSensor.getDistance();
        is_discovery = (distance_mm >= 0 && distance_mm <= target_distance);

        // 指定したミリ秒間隔 (log_interval_ms) ごとにログ出力
        if (++log_counter >= log_threshold) {
          printf("distance = %d, is_discovery = %s\n", (int)distance_mm, is_discovery ? "true" : "false");
          log_counter = 0;
        }

        if (is_discovery) break;
        sleep_ms(sensor_interval_ms);
      }
      if (is_discovery) break;
        
      // --- 2. 右を見渡す (+90度まで) ---
      printf("Looking right (+90 deg)...\n");
      imu.resetHeading();
      log_counter = 0;
      while (1) {
        float current_angle = imu.getHeading();
        int32_t distance_mm = ultrasonicSensor.getDistance();
        
        is_discovery = (distance_mm >= 0 && distance_mm <= target_distance);
        bool is_rotating = (current_angle >= 90.0f);

        if (++log_counter >= log_threshold) {
          printf("current_angle : %.2f, rotate to target: %s, distance = %d, is_discovery = %s\n", 
                 current_angle, is_rotating ? "True" : "False", (int)distance_mm, is_discovery ? "true" : "false");
          log_counter = 0;
        }

        if (is_rotating || is_discovery) {
          break;
        }

        leftWheel.setSpeed(rotating_speed);
        rightWheel.setSpeed(-rotating_speed);
        sleep_ms(sensor_interval_ms);
      }
      if (is_discovery) break;
      
      // --- 3. 左を見渡す (-180度まで) ---
      printf("Looking left (-180 deg)...\n");
      imu.resetHeading();
      log_counter = 0;
      while (1) {
        float current_angle = imu.getHeading();
        int32_t distance_mm = ultrasonicSensor.getDistance();
        
        is_discovery = (distance_mm >= 0 && distance_mm <= target_distance);
        bool is_rotating = (current_angle <= -180.0f);

        if (++log_counter >= log_threshold) {
          printf("current_angle : %.2f, rotate to target: %s, distance = %d, is_discovery = %s\n", 
                 current_angle, is_rotating ? "True" : "False", (int)distance_mm, is_discovery ? "true" : "false");
          log_counter = 0;
        }

        if (is_rotating || is_discovery) {
          break;
        }

        leftWheel.setSpeed(-rotating_speed);
        rightWheel.setSpeed(rotating_speed);
        sleep_ms(sensor_interval_ms);
      }
      if (is_discovery) break;
      
      // --- 4. 正面に戻る (+90度) ---
      printf("Looking straight...\n");
      imu.resetHeading();
      log_counter = 0;
      while (1) {
        float current_angle = imu.getHeading();
        int32_t distance_mm = ultrasonicSensor.getDistance();
        
        is_discovery = (distance_mm >= 0 && distance_mm <= target_distance);
        bool is_rotating = (current_angle >= 90.0f);

        if (++log_counter >= log_threshold) {
          printf("current_angle : %.2f, rotate to target: %s, distance = %d, is_discovery = %s\n", 
                 current_angle, is_rotating ? "True" : "False", (int)distance_mm, is_discovery ? "true" : "false");
          log_counter = 0;
        }

        if (is_rotating || is_discovery) {
          break;
        }

        leftWheel.setSpeed(rotating_speed);
        rightWheel.setSpeed(-rotating_speed);
        sleep_ms(sensor_interval_ms);
      }
      if (is_discovery) break;
      
      leftWheel.stop();
      rightWheel.stop();
      sleep_ms(500);
    }
    
    printf("Target discovered!\n");
    leftWheel.stop();
    rightWheel.stop();
    
    printf("Press Force Sensor to restart exploring.\n");
    while (!forceSensor.isTouched()) {
      sleep_ms(50);
    }

    printf("Restarting...\n");
  }
}