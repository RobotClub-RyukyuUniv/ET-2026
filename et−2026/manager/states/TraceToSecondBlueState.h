#ifndef TRACE_TO_SECOND_BLUE_STATE_H_
#define TRACE_TO_SECOND_BLUE_STATE_H_

#include "../IRunState.h"
#include <stdint.h>

class TraceToSecondBlueState : public IRunState {
private:
    enum class SecondBlueSubState {
        NORMAL_TRACING,
        EXTENDING       
    };

    SecondBlueSubState mSecondBlueSubState;
    uint64_t mActionStartTime;

public:
    void enter(CourseContext& ctx) override;
    RunState update(CourseContext& ctx) override;
};

#endif // TRACE_TO_SECOND_BLUE_STATE_H_