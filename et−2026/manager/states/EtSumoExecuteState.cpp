#include "EtSumoExecuteState.h"
#include <stdio.h>

void EtSumoExecuteState::enter(CourseContext& ctx) {
    printf("[EtSumoExecuteState] Entered. Starting ET-Sumo battle sequence.\n");
    mStep = Step::SEARCH_AND_ALIGN;
    
    // ボトル押し出し用の内部状態も初期化
    ctx.bottlePusher.init();
}

RunState EtSumoExecuteState::update(CourseContext& ctx) {
    switch (mStep) {
        case Step::SEARCH_AND_ALIGN: {
            // BottleSearcher::run() は内部でループ・中心合わせを完結させ、成功時に true を返す
            bool is_aligned = ctx.bottleSearcher.run();
            
            if (is_aligned) {
                printf("[EtSumoExecuteState] Bottle search and alignment completed. Moving to push action.\n");
                mStep = Step::PUSH_BOTTLE;
            } else {
                // 万が一失敗した場合のフォールバック（必要に応じて）
                printf("[EtSumoExecuteState] Bottle search failed. Retrying...\n");
            }
            break;
        }
        case Step::PUSH_BOTTLE: {
            // BottlePusher::run() は非同期（毎フレーム呼び出し型）で進み、完了時に true を返す
            bool is_pushed = ctx.bottlePusher.run();
            
            if (is_pushed) {
                printf("[EtSumoExecuteState] Bottle successfully pushed out! Returning to course.\n");
                
                // 次のステート（オドメトリと黒線検知を使ったコース復帰）へ移行
                return RunState::ET_SUMO_RETURN;
            }
            break;
        }
    }

    return RunState::ET_SUMO_EXECUTE; // 処理完了まではこのステートを継続
}