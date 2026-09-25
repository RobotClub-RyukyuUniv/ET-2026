#ifndef RETURN_ACTION_H
#define RETURN_ACTION_H

#include "Odometry.h"
#include "StraightRunner.h"
#include "../device/ColorSensorManager.h"
#include "../device/TimerManager.h"

class ReturnAction {
private:
    Odometry& odometry;
    StraightRunner& straightRunner;
    ColorSensorManager& colorSensorManager;
    TimerManager& timerManager;

    // 動作パラメータ
    static constexpr int SPEED_MOVE = 50;

public:
    ReturnAction(Odometry& odome,
                 StraightRunner& runner,
                 ColorSensorManager& colorMgr,
                 TimerManager& timerMgr);

    // 復帰動作を実行し、黒線に到達したら true を返す
    bool run();
};

#endif // RETURN_ACTION_H