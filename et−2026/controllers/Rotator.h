#ifndef ROTATOR_H
#define ROTATOR_H

#include "../device/HeadingSensorManager.h"
#include "../device/DualMotorController.h" // 左右のモーター制御用（前回までのコードの構成を想定）

class Rotator {
private:
    HeadingSensorManager& headingSensor;
    DualMotorController& motors;

public:
    // コンストラクタ：センサー管理とモーター制御の参照を受け取る
    Rotator(HeadingSensorManager& sensor, DualMotorController& motors_ref);

    // 指定された角度(angle)まで、指定された速度(speed)で旋回する
    void rotate(float angle, int speed);
};

#endif // ROTATOR_H