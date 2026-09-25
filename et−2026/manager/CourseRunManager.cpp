#ifdef alignof
#undef alignof
#endif

#include "CourseRunManager.h"
#include "states/LapTraceState.h"
#include "states/TraceToSecondBlueState.h"
#include "states/PrepareMeasureState.h"
#include "states/MeasureBottleColorState.h"
#include "states/PrepareResumeState.h"
#include "states/TraceToDestinationState.h"
#include "states/PlaceBottleState.h"
#include "states/CourseReturnState.h"

// --- 後半戦の新規ステートをインクルード ---
#include "states/ToEtSumoRouteState.h"
#include "states/ApproachCircleState.h"
#include "states/EtSumoExecuteState.h"
#include "states/EtSumoReturnState.h"
#include "states/GoalRunState.h"
#include "states/FinishedState.h"

#include <stdio.h>

CourseRunManager::CourseRunManager(LineTracer& lineTracer, 
                                   ColorSensorManager& colorSensorManager, 
                                   TimerManager& timerManager,
                                   DualMotorController& dualMotorController,
                                   ArmManager& armManager,
                                   HeadingSensorManager& headingSensor,
                                   Odometry& odometry,
                                   BottleSearcher& bottleSearcher,
                                   BottlePusher& bottlePusher,
                                   ReturnAction& returnAction)
    : mCtx(lineTracer, colorSensorManager, timerManager, dualMotorController, 
           armManager, headingSensor, odometry, bottleSearcher, bottlePusher, returnAction),
      mCurrentStateId(RunState::LAP_TRACE) {
    
    // 前半戦〜配置
    mStates[RunState::LAP_TRACE] = std::make_unique<LapTraceState>();
    mStates[RunState::TRACE_TO_SECOND_BLUE] = std::make_unique<TraceToSecondBlueState>();
    mStates[RunState::PREPARE_MEASURE] = std::make_unique<PrepareMeasureState>();
    mStates[RunState::MEASURE_BOTTLE_COLOR] = std::make_unique<MeasureBottleColorState>();
    mStates[RunState::PREPARE_RESUME] = std::make_unique<PrepareResumeState>();
    mStates[RunState::TRACE_TO_DESTINATION] = std::make_unique<TraceToDestinationState>();
    mStates[RunState::PLACE_BOTTLE] = std::make_unique<PlaceBottleState>();
    mStates[RunState::COURSE_RETURN] = std::make_unique<CourseReturnState>();

    // 後半戦 (ET相撲〜ゴール)
    mStates[RunState::TO_ET_SUMO_ROUTE] = std::make_unique<ToEtSumoRouteState>();
    mStates[RunState::APPROACH_CIRCLE] = std::make_unique<ApproachCircleState>();
    mStates[RunState::ET_SUMO_EXECUTE] = std::make_unique<EtSumoExecuteState>();
    mStates[RunState::ET_SUMO_RETURN] = std::make_unique<EtSumoReturnState>();
    mStates[RunState::GOAL_RUN] = std::make_unique<GoalRunState>();
    mStates[RunState::FINISHED] = std::make_unique<FinishedState>();
}

void CourseRunManager::init() {
    changeState(RunState::LAP_TRACE);
    printf("[CourseRunManager] Initialized. State: LAP_TRACE\n");
}

void CourseRunManager::update() {
    auto it = mStates.find(mCurrentStateId);
    if (it != mStates.end() && it->second) {
        RunState nextState = it->second->update(mCtx);
        if (nextState != mCurrentStateId) {
            changeState(nextState);
        }
    } else {
        mCtx.dualMotor.stop();
    }
}

void CourseRunManager::changeState(RunState nextStateId) {
    auto currentIt = mStates.find(mCurrentStateId);
    if (currentIt != mStates.end() && currentIt->second) {
        currentIt->second->exit(mCtx);
    }
    
    mCurrentStateId = nextStateId;
    
    auto nextIt = mStates.find(mCurrentStateId);
    if (nextIt != mStates.end() && nextIt->second) {
        nextIt->second->enter(mCtx);
    }
}