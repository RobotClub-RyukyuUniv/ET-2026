#ifndef FINISHED_STATE_H_
#define FINISHED_STATE_H_

#include "../IRunState.h"

class FinishedState : public IRunState {
public:
    void enter(CourseContext& ctx) override;
    RunState update(CourseContext& ctx) override;
};

#endif // FINISHED_STATE_H_