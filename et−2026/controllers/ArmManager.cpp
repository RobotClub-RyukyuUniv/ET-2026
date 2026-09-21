#include "ArmManager.h"

// コンストラクタの定義
ArmManager::ArmManager(ev3api::Motor& motor)
    : armMotor(motor) {
}

// + up() : void
// アームを上げる動作
void ArmManager::up() {
    // 例: PWMパワー30で指定角度まで回す、または一定時間動かす処理
    // （※値や制御方式は機体に合わせて調整してください）
    armMotor.setPWM(30); 
}

// + down() : void
// アームを下げる動作
void ArmManager::down() {
    // 例: 逆方向にPWMパワー-30で動かす処理
    armMotor.setPWM(-30);
}