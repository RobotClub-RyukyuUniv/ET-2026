#include "LapTraceState.h"
#include <stdio.h>

void LapTraceState::enter(CourseContext& ctx) {
    // 元の init() で行っていた初期化
    mTraceSubState = TraceSubState::NORMAL_TRACING;
    mActionStartTime = 0;
    ctx.lineTracer.init();
    printf("[CourseRunManager] Initialized. State: LAP_TRACE\n");
}

RunState LapTraceState::update(CourseContext& ctx) {
    // 毎フレーム共通でHSVを取得・青色判定（元の update の先頭にあった処理）
    spikeapi::ColorSensor::HSV hsv;
    ctx.colorSensor.getSurfaceHSV(hsv);
    bool is_blue = (hsv.h >= 200 && hsv.h <= 240 && hsv.s > 50 && hsv.v > 30);

    switch (mTraceSubState) {
        case TraceSubState::NORMAL_TRACING: {
            if (is_blue) {
                printf("[CourseRunManager] 1st Blue detected! Branch action phase 1...\n");
                mActionStartTime = ctx.timer.now();
                mTraceSubState = TraceSubState::BLUE_ACTION_PHASE1;
                ctx.lineTracer.resetIntegral();
                break;
            }
            LineTraceParam param = { 0.89f, 0.0f, 0.4f, 20, 100.0f, 0.0f, 3.2f, 0.1f, 0.4925f };
            ctx.lineTracer.run(param);
            break;
        }
        case TraceSubState::BLUE_ACTION_PHASE1: {
            uint64_t elapsed = ctx.timer.now() - mActionStartTime;
            if (elapsed < 3.0 * 1000 * 1000) {
                LineTraceParam param = {0.85f, 0.0f, 0.058f, 20, 38.0f, 1.0f, 0.0f, 0.0f, 0.4f};
                ctx.lineTracer.run(param);
            } else {
                mTraceSubState = TraceSubState::BLUE_ACTION_PHASE2;
            }
            break;
        }
        case TraceSubState::BLUE_ACTION_PHASE2: {
            uint64_t elapsed = ctx.timer.now() - mActionStartTime;
            if (elapsed < 3.7 * 1000 * 1000) {
                LineTraceParam param = {0.50f, 0.0f, 0.0f, 20, 40.0f, 12.0f, 0.0f, 0.0f, 0.4f};
                ctx.lineTracer.run(param);
            } else {
                printf("[CourseRunManager] Branch cleared! Moving to TRACE_TO_SECOND_BLUE.\n");
                ctx.lineTracer.resetIntegral();
                return RunState::TRACE_TO_SECOND_BLUE; // 次のステートへ遷移要求！
            }
            break;
        }
    }
    
    return RunState::LAP_TRACE; // まだこの状態を継続する
}