#ifndef ET_SUMO_RETURN_STATE_H_
#define ET_SUMO_RETURN_STATE_H_

#include "../IRunState.h"

class EtSumoReturnState : public IRunState {
public:
    void enter(CourseContext& ctx) override;
    RunState update(CourseContext& ctx) override;
};

#endif // ET_SUMO_RETURN_STATE_H_