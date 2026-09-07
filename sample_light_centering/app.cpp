#include "app.h"
#include <stdio.h>
#include <serial/newlib.h>
#include <serial/serial.h>

#include <libcpp/spike/Motor.h>
#include <libcpp/spike/ForceSensor.h>
#include <libcpp/spike/IMU.h>
#include <libcpp/spike/Clock.h>
#include <libcpp/spike/ColorSensor.h>

using namespace spikeapi;

void main_task(intptr_t unused) {
  // 標準出力の設定
  stdout = serial_open_newlib_file(SIO_USB_PORTID);

  // デバイスの初期化
  Motor leftWheel(EPort::PORT_B, Motor::EDirection::COUNTERCLOCKWISE, true);
  Motor rightWheel(EPort::PORT_A, Motor::EDirection::CLOCKWISE, true);
  ForceSensor forceSensor(EPort::PORT_D);
  ColorSensor colorSensor(EPort::PORT_E);
  IMU imu;
  Clock clock;

  // 設定値
  const int rotating_speed = 30; // 旋回速度
  const int drop_threshold = 1;  // ピークからいくつ下がったら「下がり」とみなすかの閾値
  const int margin = 2;

  printf("=== Find Object Center (Rise & Fall Average) Started ===\n");
  printf("Press Force Sensor to start.\n");

  // フォースセンサーが押されるまで待機[cite: 1]
  while (!forceSensor.isTouched()) {
    clock.sleep(50 * 1000); 
  }

  while (true) {
    printf("Scanning...\n");
    imu.resetHeading(); // 現在の向きを0度にリセット[cite: 2]

    // まず右方向へ首振りを開始
    leftWheel.setSpeed(rotating_speed);
    rightWheel.setSpeed(-rotating_speed);

    int detect_phase = 0; // 0: 上がり待ち, 1: 下がり待ち
    float start_angle = 0.0f;
    float end_angle = 0.0f;
    int32_t max_reflection = -1;
    int32_t min_ref_before = colorSensor.getReflection(); //[cite: 3]
    bool found = false;

    // 1. 首振りと「上がり」「下がり」の検知
    while (true) {
      float current_angle = imu.getHeading(); //[cite: 2]
      int32_t reflection = colorSensor.getReflection(); //[cite: 3]

      if (detect_phase == 0) {
        // 反射光が背景から上がり始めた瞬間をキャッチ（上がり）
        if (reflection > min_ref_before + margin) {
          start_angle = current_angle;
          detect_phase = 1;
          max_reflection = reflection;
        } else {
          if (reflection < min_ref_before) {
            min_ref_before = reflection; // 背景の最小反射光を追従
          }
        }
      } 
      else if (detect_phase == 1) {
        // ピークを更新し、そこから drop_threshold 以上下がった瞬間をキャッチ（下がり）
        if (reflection > max_reflection) {
          max_reflection = reflection;
        } else if (max_reflection - reflection >= drop_threshold) {
          end_angle = current_angle;
          found = true;
          printf("Rise angle: %.2f, Fall angle: %.2f\n", start_angle, end_angle);
          break;
        }
      }

      // 首振りの折り返し処理 (右に60度まで探して無ければ左へ)
      if (current_angle >= 60.0f) {
        leftWheel.setSpeed(-rotating_speed);
        rightWheel.setSpeed(rotating_speed);
      }
      // 左に60度まで探して無ければ諦めてやり直し
      else if (current_angle <= -60.0f) {
        printf("Object not found in range.\n");
        break;
      }

      clock.sleep(10 * 1000); // 10ms待機
    }

    if (!found) {
      leftWheel.stop();
      rightWheel.stop();
      printf("Press Force Sensor to retry.\n");
      while (!forceSensor.isTouched()) {
        clock.sleep(50 * 1000); //[cite: 1]
      }
      continue;
    }

    // 2. 上がった時と下がった時の「間の角度（中央）」を計算
    float target_angle = (start_angle + end_angle) / 2.0f;
    printf("Target Center Angle: %.2f\n", target_angle);

    float current_angle = imu.getHeading(); //[cite: 2]
    
    // 計算した中央の角度へ移動
    if (current_angle > target_angle) {
      // 現在位置が目標より右にある場合は左旋回で戻る
      leftWheel.setSpeed(-rotating_speed);
      rightWheel.setSpeed(rotating_speed);
      while (imu.getHeading() > target_angle) { //[cite: 2]
        clock.sleep(10 * 1000);
      }
    } else {
      // 現在位置が目標より左にある場合は右旋回で戻る
      leftWheel.setSpeed(rotating_speed);
      rightWheel.setSpeed(-rotating_speed);
      while (imu.getHeading() < target_angle) { //[cite: 2]
        clock.sleep(10 * 1000);
      }
    }

    // 停止
    leftWheel.stop();
    rightWheel.stop();
    
    printf("Target locked at center. Final angle: %.2f\n", imu.getHeading()); //[cite: 2]

    printf("Press Force Sensor to restart exploring.\n");
    while (!forceSensor.isTouched()) { //[cite: 1]
      clock.sleep(50 * 1000);
    }
  }
}