#ifndef BOTTLE_PUSHER_H_
#define BOTTLE_PUSHER_H_

#include "DualMotorController.h"
#include "ArmManager.h"
#include "Odometry.h" // 移動距離の判定用

class BottlePusher {
public:
    BottlePusher(LeftRightMotor& motors, ArmManager& arm, Odometry& odometry);

    void init();

    // UML定義メソッド[cite: 38]
    // 押し出しが完了したら true を返す
    bool run();

private:
    // UML定義属性[cite: 38]
    int pushState; 
    
    LeftRightMotor& mMotors;
    ArmManager& mArm;
    Odometry& mOdometry;
};

#endif // BOTTLE_PUSHER_H_