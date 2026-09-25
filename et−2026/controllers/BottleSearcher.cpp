#include "BottleSearcher.h"
#include <stdio.h>

BottleSearcher::BottleSearcher(BottleExtractor& extractor,
                               Rotator& rot,
                               StraightRunner& runner,
                               ObjectCenterAligner& aligner,
                               HeadingSensorManager& headingMgr,
                               DualMotorController& motorCtrl,
                               TimerManager& timerMgr)
    : bottleExtractor(extractor),
      rotator(rot),
      straightRunner(runner),
      centerAligner(aligner),
      headingSensorManager(headingMgr),
      dualMotorController(motorCtrl),
      timerManager(timerMgr),
      searchState(0) {}

void BottleSearcher::init() {
    searchState = 0;
    bottleExtractor.init(); //
}

bool BottleSearcher::run() {
    printf("[BottleSearcher] Starting bottle search sequence...\n");

    ScanData scan_buffer[MAX_SAMPLES];

    while (true) {
        TargetPhase phase = TargetPhase::NONE;

        // ==========================================
        // 1. スキャン開始位置（左 -45度）へ首振り移動
        // ==========================================
        printf("[BottleSearcher] Moving to scan start position (Left %.1f deg)...\n", SCAN_HALF_RANGE);
        headingSensorManager.resetHeading(); //[cite: 2, 8]
        dualMotorController.setSpeed(-SPEED_ROTATE, SPEED_ROTATE); //[cite: 9]

        while (headingSensorManager.getHeading() > -SCAN_HALF_RANGE) { //[cite: 2, 8]
            phase = bottleExtractor.detect(); //[cite: 3]
            if (phase != TargetPhase::NONE && phase != TargetPhase::SURE_AMBIENT) break;
            timerManager.sleep(10 * 1000); // 10ms待機[cite: 6]
        }
        dualMotorController.stop(); //[cite: 9]

        // ==========================================
        // 2. 左端 (-45度) から右端 (+45度) へスキャン
        // ==========================================
        if (phase == TargetPhase::NONE || phase == TargetPhase::SURE_AMBIENT) {
            timerManager.sleep(100 * 1000); //[cite: 6]
            phase = TargetPhase::NONE;

            printf("[BottleSearcher] Scanning environment from Left (-%.1f) to Right (+%.1f deg)...\n", SCAN_HALF_RANGE, SCAN_HALF_RANGE);
            int sample_count = 0;
            headingSensorManager.resetHeading(); //[cite: 2, 8]
            dualMotorController.setSpeed(SPEED_ROTATE, -SPEED_ROTATE); //[cite: 9]

            float total_sweep_angle = SCAN_HALF_RANGE * 2.0f;
            while (headingSensorManager.getHeading() < total_sweep_angle && sample_count < MAX_SAMPLES) { //[cite: 2, 8]
                TargetPhase temp_phase = bottleExtractor.detect(); //[cite: 3]

                scan_buffer[sample_count].angle = -SCAN_HALF_RANGE + headingSensorManager.getHeading(); //[cite: 2, 8]
                // ※サンプルデータの記録 (V値の評価用)
                scan_buffer[sample_count].v = 0; // 必要に応じて計測V値を記録
                sample_count++;

                if (temp_phase != TargetPhase::NONE && temp_phase != TargetPhase::SURE_AMBIENT) {
                    phase = temp_phase;
                    break;
                }
                timerManager.sleep(10 * 1000); //[cite: 6]
            }
            dualMotorController.stop(); //[cite: 9]

            // ==========================================
            // 3. スキャンデータの解析 (谷・影の判定)
            // ==========================================
            if (phase == TargetPhase::NONE && sample_count > 0) {
                int min_idx = 0;
                int32_t max_v = scan_buffer[0].v;
                for (int i = 1; i < sample_count; i++) {
                    if (scan_buffer[i].v < scan_buffer[min_idx].v) min_idx = i;
                    if (scan_buffer[i].v > max_v) max_v = scan_buffer[i].v;
                }

                int32_t min_v = scan_buffer[min_idx].v;
                float target_angle = scan_buffer[min_idx].angle;

                const int32_t DEPTH_THRESHOLD = 15;
                bool is_deep_enough = (max_v - min_v) > DEPTH_THRESHOLD;

                const int32_t WIDTH_TOLERANCE = 10;
                int left_idx = min_idx;
                int right_idx = min_idx;
                while (left_idx > 0 && scan_buffer[left_idx - 1].v <= (min_v + WIDTH_TOLERANCE)) left_idx--;
                while (right_idx < sample_count - 1 && scan_buffer[right_idx + 1].v <= (min_v + WIDTH_TOLERANCE)) right_idx++;

                float valley_width = scan_buffer[right_idx].angle - scan_buffer[left_idx].angle;
                const float MIN_WIDTH_DEG = 3.0f;
                const float MAX_WIDTH_DEG = SCAN_HALF_RANGE * 0.8f;
                bool is_valid_width = (valley_width >= MIN_WIDTH_DEG && valley_width <= MAX_WIDTH_DEG);

                if (is_deep_enough && is_valid_width && min_v <= 30) {
                    printf("[BottleSearcher] Target Valley Detected! Aligning direction...\n");
                    float turn_diff = SCAN_HALF_RANGE - target_angle;
                    rotator.rotate(-turn_diff, SPEED_ROTATE); //
                    phase = TargetPhase::SURE_AMBIENT;
                }
            }
        }

        // ==========================================
        // 4. ターゲット発見時: ObjectCenterAligner による中心合わせ
        // ==========================================
        if (phase != TargetPhase::NONE) {
            printf("[BottleSearcher] Target acquired. Executing center alignment...\n");
            
            // オブジェクト中心合わせの実行
            bool aligned = centerAligner.alignCenter(SPEED_ROTATE);
            
            if (aligned) {
                printf("[BottleSearcher] Bottle centered successfully!\n");
                return true; // 探索および中心合わせ成功
            } else {
                printf("[BottleSearcher] Center alignment missed. Retrying search...\n");
            }
        }

        // ==========================================
        // 5. ボトルが見つからない場合: 前進して位置変更
        // ==========================================
        printf("[BottleSearcher] Moving forward to search again...\n");
        straightRunner.run(SPEED_SEARCH); //
        timerManager.sleep(1500 * 1000); // 1.5秒前進[cite: 6]
        straightRunner.stop(); //[cite: 5]
    }

    return false;
}