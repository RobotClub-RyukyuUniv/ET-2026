#ifndef ARM_MANAGER_H
#define ARM_MANAGER_H

#include "Motor.h"

using namespace spikeapi;

class ArmManager {
private:
    // モーターのインスタンスを内部で保持
    Motor armMotor;

public:
    // コンストラクタ（ポートCの設定をここで行う）
    ArmManager();

    // アームを上げる
    void up();

    // アームを下げる
    void down();
};

#endif // ARM_MANAGER_H