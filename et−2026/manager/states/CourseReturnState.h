#ifndef COURSE_RETURN_STATE_H_
#define COURSE_RETURN_STATE_H_

#include "IRunState.h"

class CourseReturnState : public IRunState {
public:
    void enter(CourseContext& ctx) override;
    RunState update(CourseContext& ctx) override;
};

#endif // COURSE_RETURN_STATE_H_