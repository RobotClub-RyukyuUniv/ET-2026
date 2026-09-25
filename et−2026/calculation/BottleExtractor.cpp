#include "BottleExtractor.h"
#include <stdio.h> // printf用

BottleExtractor::BottleExtractor(ColorSensorManager& colorManager,
                                 UltrasonicDistanceSensorManager& usManager,
                                 float emaAlpha,
                                 size_t medianSize)
    : colorSensorManager(colorManager),
      ultrasonicManager(usManager),
      sEmaFilter(emaAlpha),     // コンポジションの初期化
      vEmaFilter(emaAlpha),
      sMedianFilter(medianSize),
      vMedianFilter(medianSize),
      thresholdRange(0.4f) {
}

void BottleExtractor::init() {
    // 起動直後の環境光を計測してEMAの初期値に反映する[cite: 5]
    ColorSensorManager::SV ambientSV;
    colorSensorManager.getAmbientSV(ambientSV);
    sEmaFilter.update(static_cast<float>(ambientSV.s));
    vEmaFilter.update(static_cast<float>(ambientSV.v));
    printf("[BottleExtractor Init] Applied initial ambient values.\n");
}

TargetPhase BottleExtractor::detect() {
    // 1. 各種センサーマネージャーから値を取得
    ColorSensorManager::SV surfaceSV;  // 発光時 (hsv)
    ColorSensorManager::SV ambientSV;  // 環境光 (hsv_out)
    
    colorSensorManager.getSurfaceSV(surfaceSV);
    colorSensorManager.getAmbientSV(ambientSV);
    int32_t reflection = colorSensorManager.getReflection();
    int32_t dist = ultrasonicManager.getDistance();

    float currentS = static_cast<float>(ambientSV.s);
    float currentV = static_cast<float>(ambientSV.v);

    // 2. メディアンフィルターの更新（app_3.cppの v_history バッファ更新に相当）[cite: 5]
    float medianS = sMedianFilter.update(currentS); // sもクラス図通り更新
    float medianV = vMedianFilter.update(currentV);

    // 判定に使うために、更新前の現在のEMA値を取得
    float emaS = sEmaFilter.get();
    float emaV = vEmaFilter.get();

    // ログ出力
    printf("[BottleExtractor] In: S=%d, V=%d | Out: S=%d, V=%d | Ref: %ld | Dist: %ld | EMA(s=%.1f, v=%.1f)\n",
           surfaceSV.s, surfaceSV.v, ambientSV.s, ambientSV.v, reflection, (long)dist, emaS, emaV);

    // ==========================================
    // 3. 優先順位付きセンサー評価ロジック (app_3.cppベース)[cite: 5]
    // ==========================================

    // ①【トドメ】至近距離：発光時のVやSが条件に一致[cite: 5]
    if ((surfaceSV.v >= 1 && surfaceSV.v <= 15) || (surfaceSV.s >= 1 && surfaceSV.s <= 20)) {
        return TargetPhase::CATCH_HSV;
    }
    
    // ②【中距離・影判定のロバスト化】動的閾値[cite: 5]
    if ((currentS < emaS * thresholdRange || currentV < emaV * thresholdRange) && emaV > 0.5f) {
        if (medianV <= 20.0f || medianS <= 10) {
            return TargetPhase::SURE_AMBIENT; // 中央値も下がっていれば本物[cite: 5]
        } else {
            // ノイズとみなしてEMAを更新し、NONEとする[cite: 5]
            sEmaFilter.update(currentS);
            vEmaFilter.update(currentV);
            return TargetPhase::NONE;
        }
    }
    
    // ③【補助】反射率の反応 ＋ EMAベースライン更新[cite: 5]
    if (reflection > 0) {
        sEmaFilter.update(currentS);
        vEmaFilter.update(currentV);
        return TargetPhase::EARLY_REFLECTION;
    }
    
    // ④【補助】超音波（遠距離） ＋ EMAベースライン更新[cite: 5]
    if (dist > 0 && dist <= 600) {
        sEmaFilter.update(currentS);
        vEmaFilter.update(currentV);
        return TargetPhase::BONUS_ULTRASONIC;
    }
    
    // ⑤ どの条件にもヒットしなかった場合[cite: 5]
    sEmaFilter.update(currentS);
    vEmaFilter.update(currentV);
    
    return TargetPhase::NONE;
}

void BottleExtractor::updateThreshold(float baseValue) {
    thresholdRange = baseValue;
}