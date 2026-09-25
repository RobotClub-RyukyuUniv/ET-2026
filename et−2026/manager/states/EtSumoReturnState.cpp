#include "EtSumoReturnState.h"
#include <stdio.h>

void EtSumoReturnState::enter(CourseContext& ctx) {
    printf("[EtSumoReturnState] Entered. Executing return action to main course.\n");
}

RunState EtSumoReturnState::update(CourseContext& ctx) {
    // ReturnAction::run() は、オドメトリと黒線検知を行いながら完了時に true を返す[cite: 21]
    bool is_returned = ctx.returnAction.run();

    if (is_returned) {
        printf("[EtSumoReturnState] Successfully returned to the black line! Switching to Goal Run.\n");
        ctx.dualMotor.stop();
        
        // コースに復帰したのでライントレーサーの積分値をリセット
        ctx.lineTracer.resetIntegral();

        return RunState::GOAL_RUN; // 最後のゴール走行ステートへ！
    }

    return RunState::ET_SUMO_RETURN;
}