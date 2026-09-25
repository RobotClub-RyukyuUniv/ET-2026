#include "ApproachCircleState.h"
#include <stdio.h>

void ApproachCircleState::enter(CourseContext& ctx) {
    printf("[ApproachCircleState] Entered. Tracing line towards the blue circle.\n");
    ctx.lineTracer.resetIntegral();
}

RunState ApproachCircleState::update(CourseContext& ctx) {
    // 1. 上部直線のライントレース（パラメータはコースに合わせて調整）
    LineTraceParam param = {0.89f, 0.0f, 0.4f, 20, 80.0f, 0.0f, 3.3f, 2.8f, 0.4925f};
    ctx.lineTracer.run(param);

    // 2. 「青い丸」の検知（カラーセンサーのHSV判定）
    spikeapi::ColorSensor::HSV hsv;
    ctx.colorSensor.getSurfaceHSV(hsv);
    bool is_blue_circle = (hsv.h >= 200 && hsv.h <= 240 && hsv.s > 50 && hsv.v > 30);

    // 3. 青い丸を発見したら、停止してET相撲（探索・押し出し）の実行ステートへ移行
    if (is_blue_circle) {
        printf("[ApproachCircleState] Blue circle detected! Starting ET-Sumo execution.\n");
        ctx.dualMotor.stop();
        
        // --- ここでオドメトリとボトル探索の初期化（バッファのクリアとセンサーリセット） ---
        ctx.odometry.reset();
        ctx.bottleSearcher.init();
        
        return RunState::ET_SUMO_EXECUTE; // 次のステートへ！
    }

    return RunState::APPROACH_CIRCLE; // 青丸を見つけるまではトレース継続
}