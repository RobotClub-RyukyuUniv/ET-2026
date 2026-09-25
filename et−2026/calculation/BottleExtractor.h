#ifndef BOTTLE_EXTRACTOR_H
#define BOTTLE_EXTRACTOR_H

#include "../device/ColorSensorManager.h"
#include "../device/UltrasonicDistanceSensorManager.h"
#include "EmaFilter.h"
#include "MedianFilter.h"

// app_3.cpp にあったターゲット発見の段階
enum class TargetPhase {
    NONE,
    BONUS_ULTRASONIC,
    EARLY_REFLECTION,
    SURE_AMBIENT,
    CATCH_HSV,
    RECOVERY
};

class BottleExtractor {
private:
    // 2つのマネージャーを参照として保持
    ColorSensorManager& colorSensorManager;
    UltrasonicDistanceSensorManager& ultrasonicManager;

    // クラス図指定の4つのフィルターをコンポジション（実体）として保持
    EmaFilter sEmaFilter;
    EmaFilter vEmaFilter;
    MedianFilter sMedianFilter;
    MedianFilter vMedianFilter;

    float thresholdRange;

public:
    // コンストラクタ（マネージャーの参照と、フィルタの設定値を受け取る）
    BottleExtractor(ColorSensorManager& colorManager, 
                    UltrasonicDistanceSensorManager& usManager, 
                    float emaAlpha = 0.2f, 
                    size_t medianSize = 5);

    // 初回のEMA基準値を取得するための初期化関数
    void init();

    // 以前の evaluate_sensors() の中身を丸ごとカプセル化した関数[cite: 5]
    TargetPhase detect();

    void updateThreshold(float baseValue);
};

#endif