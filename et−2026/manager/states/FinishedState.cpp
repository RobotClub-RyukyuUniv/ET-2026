#include "FinishedState.h"
#include <stdio.h>

void FinishedState::enter(CourseContext& ctx) {
    printf("[FinishedState] Mission Complete. All tasks finished successfully.\n");
    ctx.dualMotor.stop();
}

RunState FinishedState::update(CourseContext& ctx) {
    // 完全に停止し続け、状態は FINISHED から動かさない（遷移先なし）
    ctx.dualMotor.stop();
    return RunState::FINISHED;
}