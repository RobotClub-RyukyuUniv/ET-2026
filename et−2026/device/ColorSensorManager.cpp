#include "ColorSensorManager.h"

ColorSensorManager::ColorSensorManager()
    : colorSensor(EPort::PORT_E) {
}

// 表面（Surface）のSV値を取得
void ColorSensorManager::getSurfaceSV(SV& sv) const {
    // 関数内静的変数（データセグメントに配置され、毎回のスタック確保を避ける）
    static spikeapi::ColorSensor::HSV surfaceHsv{0, 0, 0};
    
    colorSensor.getHSV(surfaceHsv, true);
    sv.s = surfaceHsv.s;
    sv.v = surfaceHsv.v;
}

// 環境光（Ambient）のSV値を取得
void ColorSensorManager::getAmbientSV(SV& sv) const {
    static spikeapi::ColorSensor::HSV ambientHsv{0, 0, 0};
    
    colorSensor.getHSV(ambientHsv, false);
    sv.s = ambientHsv.s;
    sv.v = ambientHsv.v;
}

// h検知用(時間あったら修正)
void ColorSensorManager::getSurfaceHSV(spikeapi::ColorSensor::HSV& hsv) const {
    colorSensor.getHSV(hsv, true); // 引数trueで表面のHSVを取得
}

// 反射率を取得
int32_t ColorSensorManager::getReflection() const {
    return colorSensor.getReflection();
}