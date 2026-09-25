#include "PlaceBottleState.h"
#include <stdio.h>
#include <cmath>

void PlaceBottleState::enter(CourseContext& ctx) {
    mPlaceStep = PlaceStep::TURN_TO_AREA;
    mActionStartTime = 0;
    mMoveInDuration = 0;
    ctx.headingSensor.resetHeading();
}

RunState PlaceBottleState::update(CourseContext& ctx) {
    spikeapi::ColorSensor::HSV hsv;
    ctx.colorSensor.getSurfaceHSV(hsv);

    switch (mPlaceStep) {
        case PlaceStep::TURN_TO_AREA: {
            ctx.dualMotor.setSpeed(-20, 20);
            if (std::abs(ctx.headingSensor.getHeading()) >= 90.0f) {
                ctx.dualMotor.stop();
                mActionStartTime = ctx.timer.now();
                mPlaceStep = PlaceStep::MOVE_INTO_AREA;
            }
            break;
        }
        case PlaceStep::MOVE_INTO_AREA: {
            ctx.dualMotor.setSpeed(30, 30);
            bool in_colored_area = (hsv.s > 50 && hsv.v > 30);
            if (in_colored_area) {
                ctx.dualMotor.stop();
                printf("[CourseRunManager] Entered the target area!\n");
                mMoveInDuration = ctx.timer.now() - mActionStartTime;
                mActionStartTime = ctx.timer.now();
                mPlaceStep = PlaceStep::BACK_FROM_AREA;
            }
            break;
        }
        case PlaceStep::BACK_FROM_AREA: {
            ctx.dualMotor.setSpeed(-30, -30);
            uint64_t elapsed = ctx.timer.now() - mActionStartTime;
            if (elapsed >= mMoveInDuration) {
                ctx.dualMotor.stop();
                ctx.headingSensor.resetHeading();
                mPlaceStep = PlaceStep::TURN_TO_RETURN;
            }
            break;
        }
        case PlaceStep::TURN_TO_RETURN: {
            ctx.dualMotor.setSpeed(-20, 20);
            if (std::abs(ctx.headingSensor.getHeading()) >= 90.0f) {
                ctx.dualMotor.stop();
                printf("[CourseRunManager] Returned to line!\n");
                ctx.lineTracer.resetIntegral();
                return RunState::COURSE_RETURN; // 次の状態へ遷移！
            }
            break;
        }
    }
    return RunState::PLACE_BOTTLE; // 継続
}