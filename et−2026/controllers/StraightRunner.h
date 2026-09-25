#ifndef STRAIGHT_RUNNER_H_
#define STRAIGHT_RUNNER_H_

#include "../device/DualMotorController.h" // 左右モーター[cite: 18]

class StraightRunner {
public:
    explicit StraightRunner(DualMotorController &motors);

    // UML定義メソッド[cite: 37]
    void run(int speed);
    void stop();

private:
    DualMotorController& mMotors;
};

#endif // STRAIGHT_RUNNER_H_