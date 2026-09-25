#ifndef COLOR_SENSOR_MANAGER_H
#define COLOR_SENSOR_MANAGER_H

#include <cstdint>
#include <libcpp/spike/ColorSensor.h>

class ColorSensorManager {
public:
    // SVをクラスの内部に定義
    struct SV {
        uint8_t s;
        uint8_t v;
    };

    // コンストラクタ（引数なしで内部でポートEを固定）
    ColorSensorManager();

    // 表面（Surface）のSV値を取得
    void getSurfaceSV(SV& sv) const;

    // 環境光（Ambient）のSV値を取得
    void getAmbientSV(SV& sv) const;

    void getSurfaceHSV(spikeapi::ColorSensor::HSV &hsv) const;

    // 反射率を取得
    int32_t getReflection() const;

private:
    spikeapi::ColorSensor colorSensor;
};

#endif // COLOR_SENSOR_MANAGER_H