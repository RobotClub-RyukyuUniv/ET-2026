#include "ColorSensorManager.h"

ColorSensorManager::ColorSensorManager(spikeapi::ColorSensor& sensor)
    : colorSensor(sensor), mHsv{0, 0, 0} {
}

// 表面（Surface）のSV値を取得
void ColorSensorManager::getSurfaceSV(SV& sv) const {
    colorSensor.getHSV(mHsv, true); // メンバ変数を直接使って取得
    sv.s = mHsv.s;
    sv.v = mHsv.v;
}

// 環境光（Ambient）のSV値を取得
void ColorSensorManager::getAmbientSV(SV& sv) const {
    colorSensor.getHSV(mHsv, false); // メンバ変数を直接使って取得
    sv.s = mHsv.s;
    sv.v = mHsv.v;
}

// 反射率を取得
int32_t ColorSensorManager::getReflection() const {
    return colorSensor.getReflection();
}