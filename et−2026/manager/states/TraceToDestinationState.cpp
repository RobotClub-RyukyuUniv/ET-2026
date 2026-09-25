#include "TraceToDestinationState.h"
#include <stdio.h>

RunState TraceToDestinationState::update(CourseContext& ctx) {
    // 毎フレーム共通でHSVを取得・青色判定
    spikeapi::ColorSensor::HSV hsv;
    ctx.colorSensor.getSurfaceHSV(hsv);
    bool is_blue = (hsv.h >= 200 && hsv.h <= 240 && hsv.s > 50 && hsv.v > 30);

    // 0.5秒経過したら青マーカーのクールダウン（無視）を解除
    if (ctx.isIgnoringBlue && (ctx.timer.now() - ctx.ignoreStartTime > 500ULL * 1000)) {
        ctx.isIgnoringBlue = false;
    }

    // 青マーカーを検知し、かつ無視期間でない場合カウントアップ
    if (is_blue && !ctx.isIgnoringBlue) {
        ctx.blueMarkerCount++;
        printf("[CourseRunManager] Blue marker %d detected!\n", ctx.blueMarkerCount);
        
        ctx.isIgnoringBlue = true; 
        ctx.ignoreStartTime = ctx.timer.now();

        // ターゲット回数に到達したら配置アクションへ遷移
        if (ctx.blueMarkerCount == ctx.targetMarkerCount) {
            ctx.dualMotor.stop();
            ctx.headingSensor.resetHeading(); // 旋回用にジャイロリセット
            return RunState::PLACE_BOTTLE; 
        }
    }

    LineTraceParam param = {0.89f, 0.0f, 0.4f, 20, 80.0f, 0.0f, 3.3f, 2.8f, 0.4925f};
    ctx.lineTracer.run(param);
    
    return RunState::TRACE_TO_DESTINATION; // 継続
}