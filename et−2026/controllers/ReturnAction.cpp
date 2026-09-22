#include "ReturnAction.h"
#include <cmath>
#include <stdio.h>

ReturnAction::ReturnAction(LeftRightMotor& motors, Odometry& odometry)
    : mMotors(motors), mOdometry(odometry), returnState(0) {}

void ReturnAction::init() {
    returnState = 0; //
    printf("ReturnAction Initialized.\n");
}

bool ReturnAction::run() {
    // 目的地（原点 X:0, Y:0）
    const float targetX = 0.0f;
    const float targetY = 0.0f;

    // 現在の座標
    float currentX = mOdometry.getX();
    float currentY = mOdometry.getY();
    float currentTheta = mOdometry.getTheta();

    switch (returnState) { //[cite: 38]
        case 0: {
            // 1. 原点への目標角度を計算する
            float diffX = targetX - currentX;
            float diffY = targetY - currentY;
            float targetAngle = std::atan2(diffY, diffX);

            // 旋回速度を設定して状態移行
            mMotors.setSpeed(30, -30);
            returnState = 1; //[cite: 38]
            break;
        }
        case 1: {
            // 2. 目標角度を向くまで旋回を続ける（非ブロッキング）
            float diffX = targetX - currentX;
            float diffY = targetY - currentY;
            float targetAngle = std::atan2(diffY, diffX);
            
            // 角度の誤差が小さくなったら前進へ移行
            if (std::abs(targetAngle - currentTheta) < 0.1f) {
                mMotors.setSpeed(50, 50); // 前進
                returnState = 2; //[cite: 38]
            }
            break;
        }
        case 2: {
            // 3. 原点に到着するまで前進する
            float distanceToTarget = std::sqrt(std::pow(targetX - currentX, 2) + std::pow(targetY - currentY, 2));
            
            // 距離が十分近くなったら完了
            if (distanceToTarget < 50.0f) { // 50mm以内なら到着とする
                mMotors.stop();
                returnState = 3; //[cite: 38]
            }
            break;
        }
        case 3:
            // 4. 完了状態
            return true; //[cite: 38]
    }
    
    return false; //[cite: 38]
}