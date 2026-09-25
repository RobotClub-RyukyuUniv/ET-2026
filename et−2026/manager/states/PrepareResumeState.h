#ifndef PREPARE_RESUME_STATE_H_
#define PREPARE_RESUME_STATE_H_

#include "../IRunState.h"
#include <stdint.h>

class PrepareResumeState : public IRunState {
private:
    enum class PrepareResumeStep {
        BACK_AWAY,
        ARM_DOWN
    };

    PrepareResumeStep mResumeStep;
    uint64_t mSubStateStartTime;
    bool mIsSubStateInitialized;

public:
    void enter(CourseContext& ctx) override;
    RunState update(CourseContext& ctx) override;
};

#endif // PREPARE_RESUME_STATE_H_