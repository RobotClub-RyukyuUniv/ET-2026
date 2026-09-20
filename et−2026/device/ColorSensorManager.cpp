#include "ColorSensorManager.h"

ColorSensorManager::ColorSensorManager(spikeapi::ColorSensor& sensor)
    : colorSensor(sensor) {
}

// 列挙型 switch で処理を明確に分岐
void ColorSensorManager::getSV(SV& sv, MeasureMode mode) {
    switch (mode) {
        case MeasureMode::SURFACE:
            getSVForSurface(sv);
            break;
        case MeasureMode::AMBIENT:
            getSVForAmbient(sv);
            break;
    }
}

// 表面（SURFACE）用処理
void ColorSensorManager::getSVForSurface(SV& sv) {
    spikeapi::ColorSensor::HSV hsv;
    colorSensor.getHSV(hsv, true); // APIの surface=true を呼び出し
    
    sv.s = hsv.s;
    sv.v = hsv.v;
}

// 環境光・非表面（AMBIENT）用処理
void ColorSensorManager::getSVForAmbient(SV& sv) {
    spikeapi::ColorSensor::HSV hsv;
    colorSensor.getHSV(hsv, false); // APIの surface=false を呼び出し
    
    sv.s = hsv.s;
    sv.v = hsv.v;
}

int32_t ColorSensorManager::getReflection() {
    return colorSensor.getReflection();
}