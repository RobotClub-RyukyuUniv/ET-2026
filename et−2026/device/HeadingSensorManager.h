#ifndef HEADING_SENSOR_MANAGER_H
#define HEADING_SENSOR_MANAGER_H

#include "IMU.h" // spikeapiのIMUヘッダをインクルード

class HeadingSensorManager {
private:
    spikeapi::IMU& imu; // UMLに基づきIMUのインスタンス（参照）を保持

public:
    // コンストラクタ
    explicit HeadingSensorManager(spikeapi::IMU& imu_ref);

    // 現在の方位角（ヨー角）を取得
    float getHeading() const;

    // 方位角を0にリセット
    void resetHeading();
};

#endif // HEADING_SENSOR_MANAGER_H