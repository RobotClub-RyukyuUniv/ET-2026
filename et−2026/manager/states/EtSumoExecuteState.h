#ifndef ET_SUMO_EXECUTE_STATE_H_
#define ET_SUMO_EXECUTE_STATE_H_

#include "../IRunState.h"

class EtSumoExecuteState : public IRunState {
private:
    enum class Step {
        SEARCH_AND_ALIGN, // ボトルの探索 ＆ 中心合わせ
        PUSH_BOTTLE       // ボトルの押し出しアクション
    };

    Step mStep;

public:
    void enter(CourseContext& ctx) override;
    RunState update(CourseContext& ctx) override;
};

#endif // ET_SUMO_EXECUTE_STATE_H_