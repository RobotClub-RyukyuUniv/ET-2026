#ifndef I_RUN_STATE_H_
#define I_RUN_STATE_H_

#include "CourseContext.h"

// すべての状態のID
enum class RunState {
    LAP_TRACE,              
    TRACE_TO_SECOND_BLUE,   
    PREPARE_MEASURE,        
    MEASURE_BOTTLE_COLOR,   
    PREPARE_RESUME,         
    TRACE_TO_DESTINATION,   
    PLACE_BOTTLE,           
    COURSE_RETURN,          // 四角エリアからコースへ復帰
    
    // --- 後半戦の追加ステート ---
    TO_ET_SUMO_ROUTE,       // 青マーカー検知〜反時計回りの旋回と直線への乗り換え
    APPROACH_CIRCLE,        // 上部直線のライントレース ＆ 青い丸の検知
    ET_SUMO_EXECUTE,        // 探索・中心合わせ・押し出し
    ET_SUMO_RETURN,         // コースへの帰還
    GOAL_RUN,               // 最後のゴールエリアへ向かう
    FINISHED                
};

// 状態クラスの共通インターフェース
class IRunState {
public:
    virtual ~IRunState() = default;
    
    // 状態に入った時に1度だけ呼ばれる処理
    virtual void enter(CourseContext& ctx) {}
    
    // 毎フレーム呼ばれる処理（次に遷移すべき RunState を返す）
    virtual RunState update(CourseContext& ctx) = 0;
    
    // 状態を出る時に1度だけ呼ばれる処理
    virtual void exit(CourseContext& ctx) {}
};

#endif // I_RUN_STATE_H_