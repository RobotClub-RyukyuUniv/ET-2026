#ifndef TRACE_TO_DESTINATION_STATE_H_
#define TRACE_TO_DESTINATION_STATE_H_

#include "../IRunState.h"

class TraceToDestinationState : public IRunState {
public:
    RunState update(CourseContext& ctx) override;
};

#endif // TRACE_TO_DESTINATION_STATE_H_