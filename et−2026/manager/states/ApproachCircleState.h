#ifndef APPROACH_CIRCLE_STATE_H_
#define APPROACH_CIRCLE_STATE_H_

#include "../IRunState.h"

class ApproachCircleState : public IRunState {
public:
    void enter(CourseContext& ctx) override;
    RunState update(CourseContext& ctx) override;
};

#endif // APPROACH_CIRCLE_STATE_H_