#ifndef MEASURE_BOTTLE_COLOR_STATE_H_
#define MEASURE_BOTTLE_COLOR_STATE_H_

#include "../IRunState.h"

class MeasureBottleColorState : public IRunState {
public:
    RunState update(CourseContext& ctx) override;
};

#endif // MEASURE_BOTTLE_COLOR_STATE_H_