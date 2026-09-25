#include "Odometry.h"

Odometry::Odometry(DualMotorController& m, HeadingSensorManager& i)
    : motors(m), imu(i), logCount(0), lastLeftCount(0), lastRightCount(0), totalHeading(0.0f), currentX(0.0f), currentY(0.0f) {
}

void Odometry::reset() {
    logCount = 0;
    lastLeftCount = 0;
    lastRightCount = 0;
    totalHeading = 0.0f;
    currentX = 0.0f;
    currentY = 0.0f;

    motors.resetCount();
    imu.resetHeading();
}

// 呼び出されるだけで、クラスが勝手に今の状態を読み取ってバッファに記録する
void Odometry::recordStep() {
    if (logCount >= MAX_BUFFER_SIZE) {
        return;
    }

    // 1. 旋回などでズレた現在のIMUの角度を累積し、IMU側をリセットして誤差蓄積を防ぐ
    float current_imu_angle = imu.getHeading();
    totalHeading += current_imu_angle;
    imu.resetHeading();

    // 2. 直進等でのエンコーダの増分（平均移動量）を計算する
    int32_t currentLeft = motors.getCountLeft();
    int32_t currentRight = motors.getCountRight();
    
    float deltaLeft = (float)(currentLeft - lastLeftCount);
    float deltaRight = (float)(currentRight - lastRightCount);
    float averageCount = (deltaLeft + deltaRight) / 2.0f;

    // 3. バッファに記録
    logBuffer[logCount].encoderCount = averageCount;
    logBuffer[logCount].headingAngle = totalHeading;
    logCount++;

    // 4. 次回の差分計算のために現在のカウントを保存
    lastLeftCount = currentLeft;
    lastRightCount = currentRight;
}

// バッファをまとめて計算する
void Odometry::calculatePosition() {
    currentX = 0.0f;
    currentY = 0.0f;

    for (int i = 0; i < logCount; ++i) {
        float rad = PI * logBuffer[i].headingAngle / 180.0f;
        currentX += logBuffer[i].encoderCount * sinf(rad);
        currentY += logBuffer[i].encoderCount * cosf(rad);
    }
}

float Odometry::getRobotHeading() const {
    return totalHeading;
}

float Odometry::getAngleFromOrigin() const {
    // 基準点から見た現在地への角度を計算して返す
    float cartesian_deg = atan2f(currentY, currentX) * 180.0f / PI;
    return 90.0f - cartesian_deg;
}