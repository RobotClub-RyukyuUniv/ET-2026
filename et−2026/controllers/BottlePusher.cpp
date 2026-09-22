#include "BottlePusher.h"

BottlePusher::BottlePusher(LeftRightMotor& motors, ArmManager& arm, Odometry& odometry)
    : mMotors(motors), mArm(arm), mOdometry(odometry), pushState(0) {}

void BottlePusher::init() {
    pushState = 0; //[cite: 38]
}

bool BottlePusher::run() {
    switch (pushState) { //[cite: 38]
        case 0:
            mArm.down(); // アームを下ろしてホールド[cite: 13]
            pushState = 1; //[cite: 38]
            break;
        case 1:
            mMotors.setSpeed(100, 100); // フルパワーで前進[cite: 17]
            // オドメトリを使って一定距離進んだか判定するロジックなどを入れる
            if (mOdometry.getX() > 1000.0f) {
                pushState = 2; //[cite: 38]
            }
            break;
        case 2:
            mMotors.stop(); //[cite: 17]
            mArm.up(); // アームを戻す[cite: 13]
            return true; // 完了[cite: 38]
    }
    return false; //[cite: 38]
}