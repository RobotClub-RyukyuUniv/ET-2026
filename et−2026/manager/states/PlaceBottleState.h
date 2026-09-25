#ifndef PLACE_BOTTLE_STATE_H_
#define PLACE_BOTTLE_STATE_H_

#include "../IRunState.h"
#include <stdint.h>

class PlaceBottleState : public IRunState {
private:
    enum class PlaceStep {
        TURN_TO_AREA,     // 90度反時計回りに旋回
        MOVE_INTO_AREA,   // 直進して枠に入る
        BACK_FROM_AREA,   // 後退してラインに戻る
        TURN_TO_RETURN    // さらに90度反時計回りに旋回して復帰
    };

    PlaceStep mPlaceStep;
    uint64_t mActionStartTime;
    uint64_t mMoveInDuration;

public:
    void enter(CourseContext& ctx) override;
    RunState update(CourseContext& ctx) override;
};

#endif // PLACE_BOTTLE_STATE_H_