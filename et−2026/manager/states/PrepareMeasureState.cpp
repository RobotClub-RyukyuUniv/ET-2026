#include "PrepareMeasureState.h"
#include <stdio.h>

void PrepareMeasureState::enter(CourseContext& ctx) {
    mMeasureStep = PrepareMeasureStep::STOP_AND_BACK;
    mSubStateStartTime = 0;
    mIsSubStateInitialized = false;
}

RunState PrepareMeasureState::update(CourseContext& ctx) {
    if (!mIsSubStateInitialized) {
        mSubStateStartTime = ctx.timer.now();
        mMeasureStep = PrepareMeasureStep::STOP_AND_BACK;
        mIsSubStateInitialized = true;
    }
    
    uint64_t elapsed = ctx.timer.now() - mSubStateStartTime;
    
    switch (mMeasureStep) {
        case PrepareMeasureStep::STOP_AND_BACK:
            ctx.dualMotor.setSpeed(-15, -15);
            if (elapsed >= 550ULL * 1000) {
                ctx.dualMotor.stop();
                mSubStateStartTime = ctx.timer.now();
                mMeasureStep = PrepareMeasureStep::ARM_UP;
            }
            break;
            
        case PrepareMeasureStep::ARM_UP:
            ctx.arm.up();
            mMeasureStep = PrepareMeasureStep::APPROACH_BOTTLE;
            break;
            
        case PrepareMeasureStep::APPROACH_BOTTLE:
            ctx.dualMotor.setSpeed(15, 15);
            if (elapsed >= 400ULL * 1000) {
                ctx.dualMotor.stop();
                return RunState::MEASURE_BOTTLE_COLOR; // 次の状態へ遷移！
            }
            break;
    }
    
    return RunState::PREPARE_MEASURE; // 継続
}