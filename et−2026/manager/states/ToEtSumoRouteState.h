#ifndef TO_ET_SUMO_ROUTE_STATE_H_
#define TO_ET_SUMO_ROUTE_STATE_H_

#include "../IRunState.h"
#include <stdint.h>

class ToEtSumoRouteState : public IRunState {
private:
    enum class Step {
        TURN_COUNTER_CLOCKWISE, // 反時計回りに旋回
        MOVE_TO_LINE            // 直線へ向けて移動
    };

    Step mStep;
    uint64_t mActionStartTime;

public:
    void enter(CourseContext& ctx) override;
    RunState update(CourseContext& ctx) override;
};

#endif // TO_ET_SUMO_ROUTE_STATE_H_