#include "CourseReturnState.h"
#include <stdio.h>

void CourseReturnState::enter(CourseContext& ctx) {
    ctx.lineTracer.resetIntegral();
}

RunState CourseReturnState::update(CourseContext& ctx) {
    // 毎フレーム共通でHSVを取得・青色判定
    spikeapi::ColorSensor::HSV hsv;
    ctx.colorSensor.getSurfaceHSV(hsv);
    bool is_blue = (hsv.h >= 200 && hsv.h <= 240 && hsv.s > 50 && hsv.v > 30);

    // 青マーカーを検知したら、すぐに停止して次の状態へ遷移！
    if (is_blue) {
        printf("[CourseReturnState] Blue marker detected! Switching to ET-Sumo route.\n");
        ctx.dualMotor.stop();
        return RunState::TO_ET_SUMO_ROUTE; 
    }

    // 通常のライントレース（パラメータはご指定のものをそのまま使用）
    LineTraceParam param = {0.89f, 0.0f, 0.4f, 20, 80.0f, 0.0f, 3.3f, 2.8f, 0.4925f};
    ctx.lineTracer.run(param);

    return RunState::COURSE_RETURN; // 継続
}