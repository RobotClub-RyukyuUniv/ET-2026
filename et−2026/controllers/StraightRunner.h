#ifndef STRAIGHT_RUNNER_H_
#define STRAIGHT_RUNNER_H_

#include "DualMotorController.h" // 左右モーター[cite: 18]

class StraightRunner {
public:
    explicit StraightRunner(LeftRightMotor& motors);

    // UML定義メソッド[cite: 37]
    void run(int speed);
    void stop();

private:
    LeftRightMotor& mMotors;
};

#endif // STRAIGHT_RUNNER_H_