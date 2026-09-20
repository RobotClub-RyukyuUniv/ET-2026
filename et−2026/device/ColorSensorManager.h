#ifndef COLOR_SENSOR_MANAGER_H
#define COLOR_SENSOR_MANAGER_H

#include <cstdint>
#include "ColorSensor.h"

// SV（彩度・明度）の構造体
struct SV {
    uint8_t s;
    uint8_t v;
};

class ColorSensorManager {
private:
    spikeapi::ColorSensor& colorSensor;

    // 呼び出しごとのスタック確保を避けるため、作業用変数をメンバとして静的に保持
    mutable spikeapi::ColorSensor::HSV mHsv;

public:
    // コンストラクタ
    explicit ColorSensorManager(spikeapi::ColorSensor& sensor);

    // 表面（Surface）のSV値を取得
    void getSurfaceSV(SV& sv) const;

    // 環境光（Ambient）のSV値を取得
    void getAmbientSV(SV& sv) const;

    // 反射率を取得
    int32_t getReflection() const;
};

#endif // COLOR_SENSOR_MANAGER_H