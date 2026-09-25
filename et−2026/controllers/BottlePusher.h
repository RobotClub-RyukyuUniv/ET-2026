#ifndef BOTTLE_PUSHER_H_
#define BOTTLE_PUSHER_H_

#include "../device/DualMotorController.h"
#include "../device/ArmManager.h"
#include "../device/ColorSensorManager.h"
#include "../device/TimerManager.h"

class BottlePusher {
public:
    BottlePusher(DualMotorController& motors, ArmManager& arm, ColorSensorManager& colorSensor, TimerManager& timer);

    void init();

    // 押し出しが完了したら true を返す
    bool run();

private:
    DualMotorController& mMotors;
    ArmManager& mArm;
    ColorSensorManager& mColorSensor;
    TimerManager& mTimer;

    int pushState;
    uint64_t stateTimer; // 時間計測・タイムアウト用
};

#endif // BOTTLE_PUSHER_H_