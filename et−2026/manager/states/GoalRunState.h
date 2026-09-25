#ifndef GOAL_RUN_STATE_H_
#define GOAL_RUN_STATE_H_

#include "../IRunState.h"

class GoalRunState : public IRunState {
public:
    void enter(CourseContext& ctx) override;
    RunState update(CourseContext& ctx) override;
};

#endif // GOAL_RUN_STATE_H_