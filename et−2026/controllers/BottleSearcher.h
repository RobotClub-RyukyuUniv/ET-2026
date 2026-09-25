#ifndef BOTTLE_SEARCHER_H
#define BOTTLE_SEARCHER_H

#include "BottleExtractor.h"
#include "Rotator.h"
#include "StraightRunner.h"
#include "ObjectCenterAligner.h"
#include "../device/HeadingSensorManager.h"
#include "../device/DualMotorController.h"
#include "../device/TimerManager.h"

class BottleSearcher {
private:
    BottleExtractor& bottleExtractor;
    Rotator& rotator;
    StraightRunner& straightRunner;
    ObjectCenterAligner& centerAligner;
    HeadingSensorManager& headingSensorManager;
    DualMotorController& dualMotorController;
    TimerManager& timerManager;

    int searchState; // UMLクラス図における - searchState : int

    // 探索パラメータ設定
    static constexpr float SCAN_HALF_RANGE = 45.0f;
    static constexpr int SPEED_ROTATE = 30;
    static constexpr int SPEED_SEARCH = 110;
    static constexpr int MAX_SAMPLES = 200;

    struct ScanData {
        float angle;
        int32_t v;
    };

public:
    BottleSearcher(BottleExtractor& extractor,
                   Rotator& rot,
                   StraightRunner& runner,
                   ObjectCenterAligner& aligner,
                   HeadingSensorManager& headingMgr,
                   DualMotorController& motorCtrl,
                   TimerManager& timerMgr);

    void init();
    
    // ボトルを探索・キャッチし、中心合わせまで完了したら true を返す
    bool run(); // UMLクラス図における + run() : bool
};

#endif // BOTTLE_SEARCHER_H