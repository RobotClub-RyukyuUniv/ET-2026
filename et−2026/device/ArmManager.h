#ifndef ARM_MANAGER_H
#define ARM_MANAGER_H

#include "Motor.h"

using namespace spikeapi;

class ArmManager {
private:
    // モーターインスタンスへの参照
    Motor& armMotor;

public:
    // コンストラクタ（使用するモーターのインスタンスを受け取る）
    explicit ArmManager(Motor& motor);

    // アームを上げる
    void up();

    // アームを下げる
    void down();
};

#endif // ARM_MANAGER_H