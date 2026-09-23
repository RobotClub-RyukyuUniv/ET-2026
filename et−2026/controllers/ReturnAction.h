#ifndef RETURN_ACTION_H_
#define RETURN_ACTION_H_

#include "DualMotorController.h"
#include "Odometry.h"

class ReturnAction {
public:
    ReturnAction(LeftRightMotor& motors, Odometry& odometry);

    void init();

    // UML定義メソッド[cite: 38]
    // 復帰が完了したら true を返す
    bool run();

private:
    // UML定義属性[cite: 38]
    int returnState; 

    LeftRightMotor& mMotors;
    Odometry& mOdometry;
};

#endif // RETURN_ACTION_H_