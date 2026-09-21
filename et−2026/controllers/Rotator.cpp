#include "Rotator.h"
#include "Clock.h" // 待機処理用
#include <cmath>   // std::abs用

Rotator::Rotator(HeadingSensorManager& sensor, DualMotorController& motors_ref)
    : headingSensor(sensor), motors(motors_ref) {
}

void Rotator::rotate(float angle, int speed) {
    spikeapi::Clock clock;

    // 1. 旋回開始前に現在の角度をリセット
    headingSensor.resetHeading();

    // 2. 旋回方向の決定（angleが正なら右旋回、負なら左旋回とする）
    int leftPower = (angle > 0) ? speed : -speed;
    int rightPower = (angle > 0) ? -speed : speed;

    motors.setPower(leftPower, rightPower);

    // 3. 目的の角度に到達するまでループ
    while (true) {
        float currentAngle = headingSensor.getHeading();

        // 現在の角度の絶対値が、目標角度の絶対値以上になったらループを抜ける
        if (std::abs(currentAngle) >= std::abs(angle)) {
            break;
        }

        // CPU負荷を下げるための微小な待機（10ミリ秒）
        clock.sleep(10 * 1000); 
    }

    // 4. 旋回終了時にモーターを停止
    motors.stop();
}