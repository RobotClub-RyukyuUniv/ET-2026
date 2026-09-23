#include "ColorSensorManager.h"

using namespace spikeapi;

ColorSensorManager::ColorSensorManager(ColorSensor& sensor)
    : colorSensor(sensor) {
}

// 表面（Surface）のSV値を取得
void ColorSensorManager::getSurfaceSV(SV& sv) const {
    // 関数内静的変数（データセグメントに配置され、毎回のスタック確保を避ける）
    static ColorSensor::HSV surfaceHsv{0, 0, 0};
    
    colorSensor.getHSV(surfaceHsv, true);
    sv.s = surfaceHsv.s;
    sv.v = surfaceHsv.v;
}

// 環境光（Ambient）のSV値を取得
void ColorSensorManager::getAmbientSV(SV& sv) const {
    static ColorSensor::HSV ambientHsv{0, 0, 0};
    
    colorSensor.getHSV(ambientHsv, false);
    sv.s = ambientHsv.s;
    sv.v = ambientHsv.v;
}

// 反射率を取得
int32_t ColorSensorManager::getReflection() const {
    return colorSensor.getReflection();
}