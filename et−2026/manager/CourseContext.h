#ifndef COURSE_CONTEXT_H_
#define COURSE_CONTEXT_H_

#include "../controllers/LineTracer.h"
#include "../device/ColorSensorManager.h"
#include "../device/TimerManager.h"
#include "../device/DualMotorController.h"
#include "../device/ArmManager.h"
#include "../device/HeadingSensorManager.h"

// ET相撲用クラスのインクルード（パスは環境に合わせて調整してください）
#include "../calculation/Odometry.h"
#include "../controllers/BottleSearcher.h"
#include "../controllers/BottlePusher.h"
#include "../controllers/ReturnAction.h"

#include <stdint.h>

struct CourseContext {
    // 依存マネージャーへの参照
    LineTracer& lineTracer;
    ColorSensorManager& colorSensor;
    TimerManager& timer;
    DualMotorController& dualMotor;
    ArmManager& arm;
    HeadingSensorManager& headingSensor;

    // --- 新規追加：ET相撲用アクション＆オドメトリ ---
    Odometry& odometry;
    BottleSearcher& bottleSearcher;
    BottlePusher& bottlePusher;
    ReturnAction& returnAction;

    // --- ステート間で共有する変数 ---
    int targetMarkerCount;   
    int blueMarkerCount;     
    bool isIgnoringBlue;     
    uint64_t ignoreStartTime;

    // コンストラクタ
    CourseContext(LineTracer& lt, ColorSensorManager& cs, TimerManager& tm,
                  DualMotorController& dmc, ArmManager& am, HeadingSensorManager& hsm,
                  Odometry& odo, BottleSearcher& bs, BottlePusher& bp, ReturnAction& ra)
        : lineTracer(lt), colorSensor(cs), timer(tm),
          dualMotor(dmc), arm(am), headingSensor(hsm),
          odometry(odo), bottleSearcher(bs), bottlePusher(bp), returnAction(ra),
          targetMarkerCount(1), blueMarkerCount(0),
          isIgnoringBlue(false), ignoreStartTime(0) {}
};

#endif // COURSE_CONTEXT_H_