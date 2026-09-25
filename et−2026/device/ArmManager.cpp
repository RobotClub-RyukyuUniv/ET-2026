#include "ArmManager.h"
#include "Clock.h" // タイムアウト計測用

using namespace spikeapi;

// コンストラクタの定義
ArmManager::ArmManager()
    : armMotor(EPort::PORT_C, Motor::EDirection::COUNTERCLOCKWISE, true) {
}

// アームを上げる動作（上限でストールするか、1秒でタイムアウト）
void ArmManager::up() {
    const int8_t arm_pwm = 15; // 上げるときのパワー
    
    // アームを上方向に動かす
    armMotor.setPower(arm_pwm);

    Clock stall_timer;
    stall_timer.reset();

    // 上限に到達してストールするまで監視
    while (true) {
        if (armMotor.isStalled()) {
            break;
        }
        // 万が一のための安全装置（2.0秒経過したら強制抜け）[cite: 23]
        if (stall_timer.now() >= 2000000ULL) {
            break;
        }
    }

    // 上限に達したので位置を維持して固定
    armMotor.hold();
}

// アームを下げる動作（下限でストールするか、5秒でタイムアウト）
void ArmManager::down() {
    const int8_t arm_pwm = -15; // 下げるときのパワー
    
    // アームを下方向に動かす
    armMotor.setPower(arm_pwm);

    Clock stall_timer;
    stall_timer.reset();

    // 下限に到達してストールするまで監視
    while (true) {
        if (armMotor.isStalled()) {
            break;
        }
        // 万が一のための安全装置（5秒経過したら強制抜け）[cite: 23]
        if (stall_timer.now() >= 3000000ULL) {
            break;
        }
    }

    // 下限に達したので位置を維持して固定
    armMotor.hold();
}