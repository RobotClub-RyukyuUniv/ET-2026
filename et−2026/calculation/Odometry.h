#ifndef ODOMETRY_H
#define ODOMETRY_H

#include "../device/DualMotorController.h"
#include "../device/HeadingSensorManager.h"
#include <cmath>
#include <stdint.h>

// 1ステップごとのログ構造体（画像2の設計通り）
struct MovementLog {
    float encoderCount;
    float headingAngle;
};

class Odometry {
private:
    DualMotorController& motors;
    HeadingSensorManager& imu;

    static const int MAX_BUFFER_SIZE = 100;
    MovementLog logBuffer[MAX_BUFFER_SIZE];
    int logCount;

    // 差分計算・累積用の内部変数
    int32_t lastLeftCount;
    int32_t lastRightCount;
    float totalHeading; // テストコードの total_angle に相当

    // 計算結果の座標
    float currentX;
    float currentY;

    const float PI = 3.14159265f;

public:
    Odometry(DualMotorController& m, HeadingSensorManager& i);

    void reset();               // 初期化（カウントやIMU、累積角度をリセット）
    void recordStep();          // ★クラス内部でモーター・IMUを読み取り、差分と角度を勝手にバッファに積む
    void calculatePosition();   // 溜まったバッファを一括計算する

    // 取得用メソッド
    float getAngleFromOrigin() const;   // 基準点から見た移動後の角度を返す
    float getRobotHeading() const;      // 機体の最終的な向きを返す
};

#endif