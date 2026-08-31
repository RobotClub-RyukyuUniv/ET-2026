#include "app.h"
#include <stdio.h>
#include <serial/newlib.h>
#include <serial/serial.h>

#include <libcpp/spike/Motor.h>
#include <libcpp/spike/ForceSensor.h>
#include <libcpp/spike/UltrasonicSensor.h>
#include <libcpp/spike/Clock.h>
#include <libcpp/spike/Display.h>

using namespace spikeapi;

void main_task(intptr_t unused)
{
  // USBシリアルポートを標準出力(printf)に割り当てる
  stdout = serial_open_newlib_file(SIO_USB_PORTID);

  Motor leftWheel(EPort::PORT_B, Motor::EDirection::COUNTERCLOCKWISE, true);
  Motor rightWheel(EPort::PORT_A, Motor::EDirection::CLOCKWISE, true);
  ForceSensor forceSensor(EPort::PORT_D);
  UltrasonicSensor ultrasonicSensor(EPort::PORT_F);
  Clock clock;
  Display display;

  // 旋回速度はゆっくりにする（通り過ぎ防止のため 15〜20 程度がおすすめ）
  const int8_t search_speed = 10;

  // ペットボトルと見なす距離のしきい値（例：700mm以内なら見つけたと判断）
  const int target_distance = 400;
  bool is_discovery = false;

  // 間引き用のカウンタ変数を追加
  int log_counter = 0;

  while (1)
  {
    int32_t distance_mm = ultrasonicSensor.getDistance();

    // ディスプレイへの距離表示
    if (distance_mm >= 0) {
      int8_t distance_cm = (int8_t)(distance_mm / 10);
      if (distance_cm > 99)
        distance_cm = 99;
      display.showNumber(distance_cm);
    }

    // --- 探索と停止のロジック ---
    if (distance_mm != -1 && distance_mm <= target_distance) {
      // 正常に距離が測れており、かつ設定した距離以内に何かを発見した！
      // ＝ペットボトルの方を向いたので停止する
      is_discovery = true;
      leftWheel.stop();
      rightWheel.stop();
    } else {
      // 距離が遠すぎる、または測定エラー（<0）の時は、まだ見つかっていない
      // ＝右に旋回して探し続ける
      is_discovery = false;
      leftWheel.setSpeed(-search_speed);
      rightWheel.setSpeed(search_speed);
    }

    // ★ 50ms × 10回 = 500ms（0.5秒）に1回だけログ出力する
    if (++log_counter >= 10)
    {
      printf("distance = %d, is_discovery = %s\n", (int)distance_mm, is_discovery ? "true" : "false"); // ついでに改行(\n)も補完しました
      log_counter = 0;
    }

    // 50msごとに判定（これ以上間隔を長くすると通り過ぎやすくなります）
    clock.sleep(100);
  }

  while (forceSensor.isTouched())
  {
    ;
  }

  ext_tsk();
}