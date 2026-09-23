#include "HeadingSensorManager.h"

HeadingSensorManager::HeadingSensorManager(spikeapi::IMU& imu_ref)
    : imu(imu_ref) {
}

float HeadingSensorManager::getHeading() const {
    // IMU.h の定義に合わせて方位角を取得
    return imu.getHeading();
}

void HeadingSensorManager::resetHeading() {
    // IMU.h の定義に合わせて方位角をリセット
    imu.resetHeading();
}