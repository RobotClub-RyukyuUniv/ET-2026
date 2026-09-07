#include "app.h"
#include <stdio.h>
#include <serial/newlib.h>
#include <serial/serial.h>

#include <libcpp/spike/Motor.h>
#include <libcpp/spike/ForceSensor.h>
#include <libcpp/spike/UltrasonicSensor.h>
#include <libcpp/spike/IMU.h>
#include <libcpp/spike/Clock.h>
#include <libcpp/spike/ColorSensor.h>

using namespace spikeapi;

// ターゲット発見の段階
enum class TargetPhase {
    NONE,                 // 0: 探索中
    BONUS_ULTRASONIC,     // 1: 超音波（遠距離・早期発見のヒント）
    EARLY_REFLECTION,     // 2: 反射率（補助的な気付き）
    SURE_AMBIENT,         // 3: 環境光の動的閾値判定（影に入った！）
    CATCH_HSV,            // 4: 発光時HSVが0（至近距離・トドメ）
    RECOVERY              // 5: 見失い直後のリカバリ（小首振り）
};

void main_task(intptr_t unused) {
  stdout = serial_open_newlib_file(SIO_USB_PORTID);

  Motor leftWheel(EPort::PORT_B, Motor::EDirection::COUNTERCLOCKWISE, true);
  Motor rightWheel(EPort::PORT_A, Motor::EDirection::CLOCKWISE, true);
  ForceSensor forceSensor(EPort::PORT_D);
  UltrasonicSensor ultrasonicSensor(EPort::PORT_F);
  ColorSensor colorSensor(EPort::PORT_E);
  IMU imu;
  Clock clock;

  // 速度設定
  const int speed_search = 180;      // 直進速度
  const int speed_rotate = 90;       // 旋回速度（首振り用）
  const int speed_far = 120;         
  const int speed_approach = 50;     // 影に入って「ほぼ確定」した時の慎重な接近速度

  auto sleep_ms = [&clock](uint32_t ms) { clock.sleep(ms * 1000); };

  ColorSensor::HSV hsv, hsv_out;
  int32_t reflection;

  // ==========================================
  // 最初の計測をEMAの初期値に反映
  // ==========================================
  colorSensor.getHSV(hsv_out, false); // 起動直後の環境光を計測
  float ema_s = static_cast<float>(hsv_out.s); // 彩度の初期値に反映
  float ema_v = static_cast<float>(hsv_out.v); // 明度の初期値に反映
  const float alpha = 0.2f;    // 追従係数

  printf("[Init] First measurement applied: ema_s=%.1f, ema_v=%.1f\n", ema_s, ema_v);

  // ==========================================
  // 優先順位付きセンサー評価関数（移動中央値によるロバスト化適用版）
  // ==========================================
  auto evaluate_sensors = [&]() -> TargetPhase {
      colorSensor.getHSV(hsv);            // 発光時のHSV
      colorSensor.getHSV(hsv_out, false); // 環境光のHSV
      reflection = colorSensor.getReflection();
      int32_t dist = ultrasonicSensor.getDistance();

      // --- 移動中央値（カーネルサイズ5）のための静的バッファ ---
      static int32_t v_history[3] = {20, 20, 20}; // 初期値は適当な明るい値
      static int v_index = 0;

      // 新しい環境光の明度をバッファに追加（リングバッファ）
      v_history[v_index] = hsv_out.v;
      v_index = (v_index + 1) % 5;

      // 取得したセンサー値の確認用ログ（EMAの値も追加）
      printf("[Sensor] HSV(In): h=%d, s=%d, v=%d | HSV(Out): h=%d, s=%d, v=%d | Ref: %ld | Dist: %ld | EMA(s=%.1f, v=%.1f)\n",
             hsv.h, hsv.s, hsv.v, hsv_out.h, hsv_out.s, hsv_out.v, reflection, (long)dist, ema_s, ema_v);

      printf("         -> [RingBuffer] v: [%ld, %ld, %ld] (next_idx: %d)\n",
             (long)v_history[0], (long)v_history[1], (long)v_history[2], v_index);

      // ①【トドメ】至近距離：発光時のVやSが条件に一致
      if ((hsv.v >= 1 && hsv.v <= 15) || (hsv.s >= 1 && hsv.s <= 20)) {
          printf(" -> [Match ①] CATCH_HSV (v=%d, s=%d)\n", hsv.v, hsv.s);
          return TargetPhase::CATCH_HSV;
      }
      // ②【中距離・影判定のロバスト化】動的閾値（環境光の変化による判定）
      else if ((hsv_out.s < ema_s * 0.4f || hsv_out.v < ema_v * 0.4f) && ema_v > 0.5f) {
          
          // 直近5回の中央値（Median）を計算して「本物度」を慎重に検証
          int32_t temp_sorted[3];
          for(int i=0; i<3; i++) temp_sorted[i] = v_history[i];
          
          // バブルソートで中央値（5要素の3番目、インデックス2）を求める
          for(int i=0; i<2; i++) {
              for(int j=0; j<2-i; j++) {
                  if(temp_sorted[j] > temp_sorted[j+1]) {
                      int32_t tmp = temp_sorted[j];
                      temp_sorted[j] = temp_sorted[j+1];
                      temp_sorted[j+1] = tmp;
                  }
              }
          }
          int32_t median_v = temp_sorted[1]; // カーネル5の中央値

          // 中央値も0付近（例: 3以下など）まで本当に落ちているか？
          if (median_v <= 10) {
              printf(" -> [Robust Match] SURE_AMBIENT confirmed! (Median V = %ld)\n", (long)median_v);
              return TargetPhase::SURE_AMBIENT;
          } else {
              // 値は下がったが、中央値が0付近ではない ＝ 「ただの場所違い（偽物）」
              printf(" -> [False Alarm Ignored] Dropped, but Median V is %ld (Not near 0). Treating as NONE.\n", (long)median_v);
              // 偽物なのでEMAを緩やかにこの暗さに馴染ませておく
              ema_s = alpha * hsv_out.s + (1.0f - alpha) * ema_s;
              ema_v = alpha * hsv_out.v + (1.0f - alpha) * ema_v;
              return TargetPhase::NONE;
          }
      }
      // ③【補助】反射率の反応 ＋ EMAベースライン更新
      else if (reflection > 0) {
          ema_s = alpha * hsv_out.s + (1.0f - alpha) * ema_s;
          ema_v = alpha * hsv_out.v + (1.0f - alpha) * ema_v;
          printf(" -> [Match ③] EARLY_REFLECTION (reflection=%ld) -> EMA Updated\n", reflection);
          return TargetPhase::EARLY_REFLECTION;
      }
      // ④【補助】超音波（遠距離） ＋ EMAベースライン更新
      else if (dist > 0 && dist <= 450) {
          ema_s = alpha * hsv_out.s + (1.0f - alpha) * ema_s;
          ema_v = alpha * hsv_out.v + (1.0f - alpha) * ema_v;
          printf(" -> [Match ④] BONUS_ULTRASONIC (dist=%ld) -> EMA Updated\n", (long)dist);
          return TargetPhase::BONUS_ULTRASONIC;
      }
      
      // どの条件にもヒットしなかった場合（探索中のベースライン学習・更新）
      ema_s = alpha * hsv_out.s + (1.0f - alpha) * ema_s;
      ema_v = alpha * hsv_out.v + (1.0f - alpha) * ema_v;

      printf(" -> [Match None] TargetPhase::NONE -> EMA Updated\n");
      return TargetPhase::NONE;
  };

  printf("=== Ambient-Driven Fusion Logic with Recovery Phase Started ===\n");

  while (true) {
    TargetPhase phase = TargetPhase::NONE;

    // ==========================================
    // [A] 探索フェーズ（首振りスタイル）
    // ==========================================
    while (phase == TargetPhase::NONE) {
      // --- 1. 直進しながら探す ---
      printf("Moving forward...\n");
      leftWheel.setSpeed(speed_search); 
      rightWheel.setSpeed(speed_search);
      
      uint32_t move_elapsed = 0;
      const uint32_t move_duration = 100; // 0.1秒
      while (move_elapsed < move_duration) {
        phase = evaluate_sensors();
        if (phase != TargetPhase::NONE) break;
        sleep_ms(10);
        move_elapsed += 10;
      }
      if (phase != TargetPhase::NONE) break;

      // --- 2. 右を見渡す (+90度まで) ---
      printf("Looking right (+90 deg)...\n");
      imu.resetHeading();
      leftWheel.setSpeed(speed_rotate); 
      rightWheel.setSpeed(-speed_rotate);
      while (imu.getHeading() < 90.0f) {
        phase = evaluate_sensors();
        if (phase != TargetPhase::NONE) break;
        sleep_ms(10);
      }
      if (phase != TargetPhase::NONE) break;

      // --- 3. 左を見渡す (-180度まで) ---
      printf("Looking left (-180 deg)...\n");
      imu.resetHeading();
      leftWheel.setSpeed(-speed_rotate); 
      rightWheel.setSpeed(speed_rotate);
      while (imu.getHeading() > -180.0f) {
        phase = evaluate_sensors();
        if (phase != TargetPhase::NONE) break;
        sleep_ms(10);
      }
      if (phase != TargetPhase::NONE) break;

      // --- 4. 正面に戻る (+90度) ---
      printf("Looking straight...\n");
      imu.resetHeading();
      leftWheel.setSpeed(speed_rotate); 
      rightWheel.setSpeed(-speed_rotate);
      while (imu.getHeading() < 90.0f) {
        phase = evaluate_sensors();
        if (phase != TargetPhase::NONE) break;
        sleep_ms(10);
      }
      if (phase != TargetPhase::NONE) break;
    }

    // ==========================================
    // [B] 接近フェーズ ＆ [C] リカバリフェーズ
    // ==========================================
    bool search_again = false;

    while (true) {
        printf("Target detected! Approaching...\n");

        // トドメ（CATCH_HSV）になるか、見失う（NONE）まで進む
        while (phase != TargetPhase::NONE && phase != TargetPhase::CATCH_HSV) {
            if (phase == TargetPhase::SURE_AMBIENT) {
                leftWheel.setSpeed(speed_approach); 
                rightWheel.setSpeed(speed_approach);
            } else {
                leftWheel.setSpeed(speed_far); 
                rightWheel.setSpeed(speed_far);
            }

            sleep_ms(10);
            phase = evaluate_sensors(); // 常に状態を更新
        }

        // 捕捉成功の場合
        if (phase == TargetPhase::CATCH_HSV) {
            leftWheel.stop();
            rightWheel.stop();
            printf("★★★ Target Caught! (HSV reached 0) ★★★\n");
            search_again = false;
            break;
        }

        // --- ここからリカバリフェーズ（見失った瞬間の小首振り） ---
        leftWheel.stop();
        rightWheel.stop();
        printf("Target lost temporarily. Starting recovery scan (±15 deg)...\n");

        bool recovered = false;

        // 1. まず右に30度首を振って探す
        imu.resetHeading();
        leftWheel.setSpeed(speed_rotate);
        rightWheel.setSpeed(-speed_rotate);
        while (imu.getHeading() < 30.0f) {
            phase = evaluate_sensors();
            if (phase != TargetPhase::NONE) { recovered = true; break; }
            sleep_ms(10);
        }
        leftWheel.stop(); rightWheel.stop();

        // 2. 見つからなければ、左に60度首を振る (+30度から -30度方向へ)
        if (!recovered) {
            imu.resetHeading();
            leftWheel.setSpeed(-speed_rotate);
            rightWheel.setSpeed(speed_rotate);
            while (imu.getHeading() > -60.0f) {
                phase = evaluate_sensors();
                if (phase != TargetPhase::NONE) { recovered = true; break; }
                sleep_ms(10);
            }
            leftWheel.stop(); rightWheel.stop();
        }

        // 3. それでも見つからなければ、正面に戻す (+30度戻して中央へ)
        if (!recovered) {
            imu.resetHeading();
            leftWheel.setSpeed(speed_rotate);
            rightWheel.setSpeed(-speed_rotate);
            while (imu.getHeading() < 30.0f) {
                phase = evaluate_sensors();
                if (phase != TargetPhase::NONE) { recovered = true; break; }
                sleep_ms(10);
            }
            leftWheel.stop(); rightWheel.stop();
        }

        // リカバリの判定
        if (recovered) {
            printf("★★★ Recovery Successful! Resuming approach... ★★★\n");
            continue; 
        } else {
            printf("Recovery failed. Returning to search...\n");
            search_again = true;
            break; 
        }
    }

    if (search_again) {
        continue;
    }
    
    // 再スタート待機
    printf("Press Force Sensor to restart.\n");
    while (!forceSensor.isTouched()) {
      sleep_ms(50);
    }
    sleep_ms(500);
  }
}