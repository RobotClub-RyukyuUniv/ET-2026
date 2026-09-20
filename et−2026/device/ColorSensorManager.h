#ifndef COLOR_SENSOR_MANAGER_H
#define COLOR_SENSOR_MANAGER_H

#include <cstdint>
#include "ColorSensor.h" // ColorSensor.h をインクルード

// SV（彩度・明度）の構造体
struct SV {
    uint8_t s;
    uint8_t v;
};

class ColorSensorManager {
public:
    // true / false の代わりに意味がわかりやすい名前（列挙型）を定義
    enum class MeasureMode {
        SURFACE, // 表面の色（旧 surface = true）
        AMBIENT  // 光源・環境光（旧 surface = false）
    };

private:
    spikeapi::ColorSensor& colorSensor;

    // モード（SURFACE / AMBIENT）ごとの専用内部メソッド
    void getSVForSurface(SV& sv);
    void getSVForAmbient(SV& sv);

public:
    // コンストラクタ
    explicit ColorSensorManager(spikeapi::ColorSensor& sensor);

    // モードを指定してSV値を取得（デフォルトは SURFACE モード）
    void getSV(SV& sv, MeasureMode mode = MeasureMode::SURFACE);
    int32_t getReflection();
};

#endif // COLOR_SENSOR_MANAGER_H