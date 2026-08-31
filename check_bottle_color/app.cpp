#include "app.h"
#include <stdio.h>

// モーターは一旦考えないためコメントアウト
// #include <libcpp/spike/Motor.h>
#include <libcpp/spike/Clock.h>
#include <libcpp/spike/ForceSensor.h>
#include <libcpp/spike/ColorSensor.h> // カラーセンサーのヘッダを追加

using namespace spikeapi;

/**
 * メインタスク
 */
void main_task(intptr_t unused)
{

  // Motor leftWheel(EPort::PORT_B, Motor::EDirection::COUNTERCLOCKWISE, true);
  // Motor rightWheel(EPort::PORT_A, Motor::EDirection::CLOCKWISE, true);

  ForceSensor forceSensor(EPort::PORT_D);
  ColorSensor colorSensor(EPort::PORT_C); // ポートCにカラーセンサーを接続
  Clock clock;

  printf("Color Detection Started...\n");

  while (1) {
    // HSV構造体を用意して色を取得
    ColorSensor::HSV hsv;
    colorSensor.getHSV(hsv, true); // true = 表面の色を検出

    // 彩度(S)と明度(V)が低すぎる場合（黒や白、暗闇など）を除外するための条件
    if (hsv.s > 30 && hsv.v > 20) {

      // 色相(H)は 0〜360度 で表現されます
      // 赤: 0付近 および 360付近
      // 黄: 60付近
      // 青: 240付近
      if (hsv.h <= 20 || hsv.h >= 340) {
        printf("赤 (Red) を検出！ (H:%d S:%d V:%d)\n", hsv.h, hsv.s, hsv.v); }
      else if (hsv.h >= 40 && hsv.h <= 80) {
        printf("黄 (Yellow) を検出！ (H:%d S:%d V:%d)\n", hsv.h, hsv.s, hsv.v); }
      else if (hsv.h >= 200 && hsv.h <= 260) {
        printf("青 (Blue) を検出！ (H:%d S:%d V:%d)\n", hsv.h, hsv.s, hsv.v);
      }
    }

    // フォースセンサーが押されているかどうか調べる（ループ終了条件）
    if (forceSensor.isTouched()) {
      break;
    }

    // ログが連続して出すぎるのを防ぐため0.5秒待機
    clock.sleep(500 * 1000);
  }

  printf("Stopped.\n");

  // leftWheel.stop();
  // rightWheel.stop();

  // フォースセンサーの押された状態が解除されたかを調べる
  while (forceSensor.isTouched()) {
    clock.sleep(10 * 1000); // 10ミリ秒スリープを追加（CPUの無駄使い防止）
  }

  ext_tsk();
}