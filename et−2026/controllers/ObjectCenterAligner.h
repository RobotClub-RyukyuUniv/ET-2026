#ifndef OBJECT_CENTER_ALIGNER_H
#define OBJECT_CENTER_ALIGNER_H

#include "../device/ColorSensorManager.h"
#include "../device/HeadingSensorManager.h"
#include "../device/DualMotorController.h"
#include "../device/TimerManager.h"

class ObjectCenterAligner {
private:
    ColorSensorManager& colorSensorManager;
    HeadingSensorManager& headingSensorManager;
    DualMotorController& dualMotorController;
    TimerManager& timerManager;

public:
    // コンストラクタで各マネージャーの参照をインジェクション
    ObjectCenterAligner(ColorSensorManager& colorManager,
                          HeadingSensorManager& headingManager,
                          DualMotorController& motorController,
                          TimerManager& timerMgr);

    // オブジェクトの中心を探索して正確に揃える処理
    // 成功した場合は true、範囲内に見つからなかった場合は false を返す
    bool alignCenter(int speed = 30, int dropThreshold = 1, int margin = 2);
};

#endif // OBJECT_CENTER_ALIGNER_H