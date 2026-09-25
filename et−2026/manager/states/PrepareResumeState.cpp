#include "PrepareResumeState.h"
#include <stdio.h>

void PrepareResumeState::enter(CourseContext& ctx) {
    mResumeStep = PrepareResumeStep::BACK_AWAY;
    mSubStateStartTime = 0;
    mIsSubStateInitialized = false;
}

RunState PrepareResumeState::update(CourseContext& ctx) {
    if (!mIsSubStateInitialized) {
        mSubStateStartTime = ctx.timer.now();
        mResumeStep = PrepareResumeStep::BACK_AWAY;
        mIsSubStateInitialized = true;
    }
    
    uint64_t elapsed = ctx.timer.now() - mSubStateStartTime;
    
    switch (mResumeStep) {
        case PrepareResumeStep::BACK_AWAY:
            ctx.dualMotor.setSpeed(-40, -40);
            if (elapsed >= 500ULL * 1000) {
                ctx.dualMotor.stop();
                mSubStateStartTime = ctx.timer.now();
                mResumeStep = PrepareResumeStep::ARM_DOWN;
            }
            break;
            
        case PrepareResumeStep::ARM_DOWN:
            ctx.arm.down();
            ctx.lineTracer.resetIntegral();
            return RunState::TRACE_TO_DESTINATION; // 次の状態へ遷移！
    }
    
    return RunState::PREPARE_RESUME; // 継続
}