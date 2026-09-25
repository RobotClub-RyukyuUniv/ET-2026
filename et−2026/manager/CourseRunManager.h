#ifndef COURSE_RUN_MANAGER_H_
#define COURSE_RUN_MANAGER_H_

#ifdef alignof
#undef alignof
#endif

#include <memory>
#include <unordered_map>
#include "CourseContext.h"
#include "IRunState.h"

class CourseRunManager {
private:
    CourseContext mCtx;
    RunState mCurrentStateId;
    std::unordered_map<RunState, std::unique_ptr<IRunState>> mStates;

    void changeState(RunState nextStateId);

public:
    CourseRunManager(LineTracer& lineTracer, 
                     ColorSensorManager& colorSensorManager, 
                     TimerManager& timerManager,
                     DualMotorController& dualMotorController,
                     ArmManager& armManager,
                     HeadingSensorManager& headingSensor,
                     Odometry& odometry,
                     BottleSearcher& bottleSearcher,
                     BottlePusher& bottlePusher,
                     ReturnAction& returnAction);
    
    void init();
    void update();
};

#endif // COURSE_RUN_MANAGER_H_