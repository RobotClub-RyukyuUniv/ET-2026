#include "GoalRunState.h"
#include <stdio.h>

void GoalRunState::enter(CourseContext& ctx) {
    printf("[GoalRunState] Entered. Running towards the goal area.\n");
    ctx.lineTracer.resetIntegral();
}

RunState GoalRunState::update(CourseContext& ctx) {
    // 1. 最後のゴールへ向けたライントレース
    LineTraceParam param = {0.89f, 0.0f, 0.4f, 20, 80.0f, 0.0f, 3.3f, 2.8f, 0.4925f};
    ctx.lineTracer.run(param);

    // 2. ゴールエリアの検知（反射率が白領域かどうか）
    int32_t reflection = ctx.colorSensor.getReflection();
    bool is_white = (reflection >= 50); // 白い床の閾値

    // 3. 静的変数（またはメンバ変数）で「白を検知し始めた時間」を保持
    static uint64_t white_start_time = 0;
    static bool is_detecting_white = false;

    if (is_white) {
        if (!is_detecting_white) {
            // 白に入った瞬間を記録
            white_start_time = ctx.timer.now();
            is_detecting_white = true;
        } else {
            // 白い状態がどれくらい続いているか計算（例: 0.3秒＝300,000μs）
            uint64_t elapsed_white = ctx.timer.now() - white_start_time;
            if (elapsed_white >= 300ULL * 1000) {
                printf("[GoalRunState] Reached the goal area stably! Finished.\n");
                ctx.dualMotor.stop();
                is_detecting_white = false;
                return RunState::FINISHED; // 完全にゴール！
            }
        }
    } else {
        // 黒線に戻ったらタイマーをリセット（通常のライントレース中の白はここでリセットされる）
        is_detecting_white = false;
    }

    return RunState::GOAL_RUN;
}