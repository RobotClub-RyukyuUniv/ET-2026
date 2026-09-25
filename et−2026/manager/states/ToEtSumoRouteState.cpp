#include "ToEtSumoRouteState.h"
#include <stdio.h>
#include <cmath>

void ToEtSumoRouteState::enter(CourseContext& ctx) {
    mStep = Step::TURN_COUNTER_CLOCKWISE;
    mActionStartTime = 0;
    ctx.headingSensor.resetHeading(); // 旋回用にジャイロをリセット
}

RunState ToEtSumoRouteState::update(CourseContext& ctx) {
    switch (mStep) {
        case Step::TURN_COUNTER_CLOCKWISE: {
            // 反時計回りに旋回（スピードは機体に合わせて調整してください）
            ctx.dualMotor.setSpeed(-20, 20);
            
            // 例：90度反時計回りに旋回したら完了
            if (std::abs(ctx.headingSensor.getHeading()) >= 90.0f) {
                ctx.dualMotor.stop();
                mActionStartTime = ctx.timer.now();
                mStep = Step::MOVE_TO_LINE;
            }
            break;
        }
        case Step::MOVE_TO_LINE: {
            // 上部の直線ラインに向かって少し前進、あるいはラインに乗るまでの動作
            ctx.dualMotor.setSpeed(30, 30);
            
            // 一定時間（例: 1秒）直進して直線に乗ったら、次のライントレース状態へバトンタッチ
            uint64_t elapsed = ctx.timer.now() - mActionStartTime;
            if (elapsed >= 1000ULL * 1000) {
                ctx.dualMotor.stop();
                ctx.lineTracer.resetIntegral();
                printf("[ToEtSumoRouteState] Moved to the target line. Starting approach trace.\n");
                return RunState::APPROACH_CIRCLE; // 次のステート（上部直線のトレースへ）
            }
            break;
        }
    }
    return RunState::TO_ET_SUMO_ROUTE; // 継続
}