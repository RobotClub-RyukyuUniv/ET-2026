#include "app.h"
#include <stdio.h>

#include "Tracer.h"
#include "ForceSensor.h"
#include "Clock.h"

Tracer tracer;
Clock clock;

using namespace spikeapi;

void tracer_task(intptr_t exinf) {
  tracer.run();
  ext_tsk();
}

void main_task(intptr_t unused)
{
  const int32_t duration = 100 * 1000; // 0.1秒
  ForceSensor forceSensor(EPort::PORT_D);

  tracer.init();

  bool is_running = false;
  bool prev_touched = false;

  Clock press_timer;         // 押している時間を測るためのClockインスタンス
  bool is_measuring = false; // 計測中かどうかのフラグ

  while (true) {
    bool current_touched = forceSensor.isPressed(10.0);

    // 押された瞬間
    if (current_touched && !prev_touched) {
      press_timer.reset(); // タイマーをリセットして計測開始
      is_measuring = true;
    }

    // 押されている最中（長押し判定）
    if (current_touched && is_measuring) {
      // 2秒（2,000,000マイクロ秒）以上押し続けたらループを抜ける
      if (press_timer.now() >= 2000000ULL) {
        break;
      }
    }

    // 離された瞬間
    if (!current_touched && prev_touched) {
      if (is_measuring) {
        // 2秒未満で離された場合は、通常の on/off 切り替え
        is_running = !is_running;

        if (is_running) {
          sta_cyc(TRACER_CYC);
        }
        else {
          stp_cyc(TRACER_CYC);
          tracer.terminate();
          tracer.init();
        }
        is_measuring = false;
      }
    }

    prev_touched = current_touched;
    clock.sleep(duration);
  }

  // === 完全に while 文を抜けた後の終了処理 ===
  if (is_running) {
    stp_cyc(TRACER_CYC);
  }
  tracer.terminate();
  ext_tsk();
}