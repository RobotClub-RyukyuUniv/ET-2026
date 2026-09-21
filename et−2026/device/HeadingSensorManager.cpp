#include "HeadingSensorManager.h"

HeadingSensorManager::HeadingSensorManager(spikeapi::IMU& imu_ref)
    : imu(imu_ref) {
}

float HeadingSensorManager::getHeading() const {
    // 実際のspikeapiの仕様に合わせてヨー角を取得する処理
    // 例: imu.getAngles() からヨー軸の値を返す想定
    float angles[3];
    imu.getAngles(angles); 
    return angles[0]; // ※環境によってYawのインデックス(0,1,2)は適宜変更してください
}

void HeadingSensorManager::resetHeading() {
    // センサーの角度をリセットする処理
    imu.reset(); 
}