#include "app.h"
#include <stdio.h>

#include <libcpp/spike/Motor.h> 
#include <libcpp/spike/Clock.h>
#include <libcpp/spike/ForceSensor.h> 
#include <libcpp/spike/Display.h>

using namespace spikeapi; 

/**
 * メインタスク
 */
void main_task(intptr_t unused) { 

  Motor leftWheel(EPort::PORT_B,Motor::EDirection::COUNTERCLOCKWISE,true); // <1>
  Motor rightWheel(EPort::PORT_A,Motor::EDirection::CLOCKWISE,true);  // <2>
  ForceSensor forceSensor(EPort::PORT_D); // <3>
  Display display;
  Clock clock; // <4>

  const int8_t pwms[7] = {60, 45, 30, 25, 23, 20, 15};
  const int NUM_PWMS = 7;

  const uint32_t duration = 2000*1000;

  int8_t index = 0;

  const int WIDTH = 5;  // 横の最大数（列数）
  const int HEIGHT = 5; // 縦の最大数（行数）
  const int TOTAL_SIZE = WIDTH * HEIGHT; // 25個
  uint8_t image[25] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
  };

  display.off(); // 画面の初期化

  while(1) {
    printf("turning...\n");

    // pwmsのサイズ（4）を超えないように安全にインデックスをループさせる
    int8_t pwm = pwms[index % NUM_PWMS];

    // 二次元の座標を計算
    int x = index % WIDTH;
    int y = index / WIDTH;

    // 現在のセルを点灯
    image[y * WIDTH + x] = 100;
    display.setImage(image);

    printf("set pwm %d (index: %d, x: %d, y: %d)\n", pwm, index, x, y);

    leftWheel.setPower(pwm); 
    rightWheel.setPower(-pwm);

    // 2秒間動かしつつ、ボタンが押されないか監視する ---
    bool isPressed = false;
    uint32_t elapsed = 0;

    while (elapsed < duration) {
      if (forceSensor.isTouched()) {
        isPressed = true;
        break; // 押されたら即座にループを抜ける
      }
      clock.sleep(100 * 1000); // 100msずつ細かく待つ
      elapsed += 100 * 1000;
    }

    // 判定はフラグ（isPressed）を見る
    if (isPressed) {
      leftWheel.stop();
      rightWheel.stop();

      clock.sleep(duration);

      // 1. 今光っているセルを消灯する
      image[y * WIDTH + x] = 0;

      // 2. インデックスを次のセルに進める（25個に達したら0に戻る）
      index = (index + 1) % TOTAL_SIZE;

      // 3. 消灯状態をディスプレイに反映
      display.setImage(image);
    }
  } 

  ext_tsk();
}
