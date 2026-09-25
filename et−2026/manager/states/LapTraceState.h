#ifndef LAP_TRACE_STATE_H_
#define LAP_TRACE_STATE_H_

#include "../IRunState.h"
#include <stdint.h>

class LapTraceState : public IRunState {
private:
    // この状態の中だけで使う細かいステップ
    enum class TraceSubState {
        NORMAL_TRACING,
        BLUE_ACTION_PHASE1,
        BLUE_ACTION_PHASE2
    };

    TraceSubState mTraceSubState;
    uint64_t mActionStartTime;

public:
    void enter(CourseContext& ctx) override;
    RunState update(CourseContext& ctx) override;
};

#endif // LAP_TRACE_STATE_H_