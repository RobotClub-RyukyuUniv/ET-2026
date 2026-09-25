#ifndef PREPARE_MEASURE_STATE_H_
#define PREPARE_MEASURE_STATE_H_

#include "../IRunState.h"
#include <stdint.h>

class PrepareMeasureState : public IRunState {
private:
    enum class PrepareMeasureStep {
        STOP_AND_BACK,
        ARM_UP,
        APPROACH_BOTTLE
    };

    PrepareMeasureStep mMeasureStep;
    uint64_t mSubStateStartTime;
    bool mIsSubStateInitialized;

public:
    void enter(CourseContext& ctx) override;
    RunState update(CourseContext& ctx) override;
};

#endif // PREPARE_MEASURE_STATE_H_