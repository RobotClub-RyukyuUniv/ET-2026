// ---------------
//      没案
// ---------------
// なんか変更がしづらくもう一度やり直したいと思ったから(深い理由はない)

#include "app.h"
#include <stdio.h>
#include <serial/newlib.h>
#include <serial/serial.h>
#include <libcpp/spike/Clock.h>
#include "Finder.h"

Finder finder;
Clock clock;

using namespace spikeapi;

// OSの周期ハンドラから50msごとに呼ばれる（超音波の取得だけを行う）
void search_task(intptr_t exinf) {
  finder.run();
  ext_tsk();
}

void main_task(intptr_t unused) {
  stdout = serial_open_newlib_file(SIO_USB_PORTID);

  finder.init();

  // 1. 超音波センサー職人（50ms周期タスク）を起動
  sta_cyc(SEARCH_CYC);

  const int target_distance = 400; // 400mm以内なら発見

  // 間引き用のカウンタ変数を追加
  int log_counter = 0;

  // 2. メインタスクが全責任を持って監視・制御するループ
  while (true) {
    // 職人が裏で更新し続けている最新の距離を取得
    int32_t current_dist = finder.getLatestDistance();

    if (current_dist != -1 && current_dist <= target_distance) {
        // 見つけたら即座に止める
        finder.stopMoving();
    } else {
        // 見つからなければ旋回し続ける
        finder.startSearching();
    }

    // ★ 50ms × 10回 = 500ms（0.5秒）に1回だけログ出力する
    if (++log_counter >= 10)
    {
      printf("distance = %d\n", (int)current_dist);
      log_counter = 0;
    }

    // メインタスクの監視間隔（10msごとにチェックする）
    // ※センサー測定（50ms）より細かくチェックすることで、見つけた瞬間にすぐ反応できる
    clock.sleep(10 * 1000); 
  }

  ext_tsk();
}