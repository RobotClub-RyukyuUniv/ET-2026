#include "ObjectCenterAligner.h"
#include <stdio.h>

ObjectCenterAligner::ObjectCenterAligner(ColorSensorManager& colorManager,
                                         HeadingSensorManager& headingManager,
                                         DualMotorController& motorController,
                                         TimerManager& timerMgr)
    : colorSensorManager(colorManager),
      headingSensorManager(headingManager),
      dualMotorController(motorController),
      timerManager(timerMgr) {}

bool ObjectCenterAligner::alignCenter(int speed, int dropThreshold, int margin) {
    printf("[ObjectCenterAligner] Scanning to align center...\n");
    headingSensorManager.resetHeading(); //[cite: 2, 8]

    // まず右方向へ首振りを開始
    dualMotorController.setSpeed(speed, -speed); //[cite: 9]

    int detect_phase = 0; // 0: 上がり待ち, 1: 下がり待ち
    float start_angle = 0.0f;
    float end_angle = 0.0f;
    int32_t max_reflection = -1;
    int32_t min_ref_before = colorSensorManager.getReflection(); //[cite: 3, 7]
    bool found = false;

    // 1. 首振りと「上がり」「下がり」の検知
    while (true) {
        float current_angle = headingSensorManager.getHeading(); //[cite: 2, 8]
        int32_t reflection = colorSensorManager.getReflection(); //[cite: 3, 7]

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
            } else if (max_reflection - reflection >= dropThreshold) {
                end_angle = current_angle;
                found = true;
                printf("[ObjectCenterAligner] Rise angle: %.2f, Fall angle: %.2f\n", start_angle, end_angle);
                break;
            }
        }

        // 首振りの折り返し処理 (右に60度まで探して無ければ左へ)
        if (current_angle >= 60.0f) {
            dualMotorController.setSpeed(-speed, speed); //[cite: 9]
        }
        // 左に60度まで探して無ければ諦めてやり直し
        else if (current_angle <= -60.0f) {
            printf("[ObjectCenterAligner] Object not found in range.\n");
            break;
        }

        timerManager.sleep(10 * 1000); // 10ms待機
    }

    if (!found) {
        dualMotorController.stop(); //[cite: 9]
        return false;
    }

    // 2. 上がった時と下がった時の「間の角度（中央）」を計算
    float target_angle = (start_angle + end_angle) / 2.0f;
    printf("[ObjectCenterAligner] Target Center Angle: %.2f\n", target_angle);

    float current_angle = headingSensorManager.getHeading(); //[cite: 2, 8]
    
    // 計算した中央の角度へ移動
    if (current_angle > target_angle) {
        // 現在位置が目標より右にある場合は左旋回で戻る
        dualMotorController.setSpeed(-speed, speed); //[cite: 9]
        while (headingSensorManager.getHeading() > target_angle) { //[cite: 2, 8]
            timerManager.sleep(10 * 1000);
        }
    } else {
        // 現在位置が目標より左にある場合は右旋回で戻る
        dualMotorController.setSpeed(speed, -speed); //[cite: 9]
        while (headingSensorManager.getHeading() < target_angle) { //[cite: 2, 8]
            timerManager.sleep(10 * 1000);
        }
    }

    // 停止
    dualMotorController.stop(); //[cite: 9]
    printf("[ObjectCenterAligner] Target locked at center. Final angle: %.2f\n", headingSensorManager.getHeading()); //[cite: 2, 8]

    return true;
}