#include "TraceToSecondBlueState.h"
#include <stdio.h>

void TraceToSecondBlueState::enter(CourseContext& ctx) {
    mSecondBlueSubState = SecondBlueSubState::NORMAL_TRACING;
    mActionStartTime = 0;
}

RunState TraceToSecondBlueState::update(CourseContext& ctx) {
    // 毎フレーム共通でHSVを取得・青色判定
    spikeapi::ColorSensor::HSV hsv;
    ctx.colorSensor.getSurfaceHSV(hsv);
    bool is_blue = (hsv.h >= 200 && hsv.h <= 240 && hsv.s > 50 && hsv.v > 30);

    switch (mSecondBlueSubState) {
        case SecondBlueSubState::NORMAL_TRACING: {
            if (is_blue) {
                printf("[CourseRunManager] 2nd Blue detected! Continuing line trace for a bit...\n");
                mActionStartTime = ctx.timer.now();
                mSecondBlueSubState = SecondBlueSubState::EXTENDING;
            } else {
                LineTraceParam param = {0.89f, 0.0f, 0.4f, 20, 80.0f, 0.0f, 3.3f, 2.8f, 0.4925f};
                ctx.lineTracer.run(param);
            }
            break;
        }
        case SecondBlueSubState::EXTENDING: {
            uint64_t elapsed = ctx.timer.now() - mActionStartTime;
            if (elapsed < 1000ULL * 1000) {
                LineTraceParam param = {0.70f, 0.0f, 0.4f, 20, 80.0f, 0.0f, 3.3f, 2.8f, 0.4925f};
                ctx.lineTracer.run(param);
            } else {
                printf("[CourseRunManager] Extension finished. Stop and prepare to measure.\n");
                ctx.dualMotor.stop();
                return RunState::PREPARE_MEASURE; // 次のステート（準備シーケンス）へ！
            }
            break;
        }
    }

    return RunState::TRACE_TO_SECOND_BLUE; // 継続
}