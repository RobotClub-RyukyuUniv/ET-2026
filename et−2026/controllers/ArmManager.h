#ifndef ARM_MANAGER_H_
#define ARM_MANAGER_H_

#include "Motor.h" // EV3/SPIKE API等のMotorクラスをインクルード

class ArmManager {
private:
    // - armMotor : Motor
    ev3api::Motor& armMotor; // モーターインスタンスへの参照（または実体）

public:
    // コンストラクタ（使用するモーターのインスタンスを受け取る）
    explicit ArmManager(ev3api::Motor& motor);

    // + up() : void
    void up();

    // + down() : void
    void down();
};

#endif // ARM_MANAGER_H_