#include "BottleSearcher.h"
#include <stdio.h>

// ヘッダに定義したコンストラクタに合わせて初期化
BottleSearcher::BottleSearcher(ColorSensorManager& color, LeftRightMotor& motors, 
                               HeadingSensorManager& heading, spikeapi::UltrasonicSensor& ultrasonic)
    : mColor(color), mMotors(motors), mHeading(heading), mUltrasonic(ultrasonic),
      searchState(0), mMoveCounter(0) {}

void BottleSearcher::init() {
    searchState = 0; //[cite: 38]
    mMoveCounter = 0;
    mMotors.stop();
}

bool BottleSearcher::run() {
    // 毎回センサーを評価する（ロジックの詳細は以前の evaluateSensors を利用）
    TargetPhase phase = evaluateSensors(); 

    if (phase == TargetPhase::CATCH_HSV) {
        // 発見・捕捉したら即座に完了を返す
        mMotors.stop();
        return true; //[cite: 38]
    }

    // 発見できていない場合は、状態に応じて探索動作を進める
    switch (searchState) { //[cite: 38]
        case 0:
            // 直進して探す状態
            mMotors.setSpeed(180, 180);
            mMoveCounter++;
            if (mMoveCounter > 10) { // 一定周期（例: 10回）進んだら次の状態へ
                mMoveCounter = 0;
                mHeading.resetHeading();
                searchState = 1; //[cite: 38]
            }
            break;

        case 1:
            // 右を向いて探す状態
            mMotors.setSpeed(90, -90);
            if (mHeading.getHeading() > 90.0f) {
                mHeading.resetHeading();
                searchState = 2; //[cite: 38]
            }
            break;

        case 2:
            // 左を向いて探す状態
            mMotors.setSpeed(-90, 90);
            if (mHeading.getHeading() < -180.0f) {
                mHeading.resetHeading();
                searchState = 3; //[cite: 38]
            }
            break;

        case 3:
            // 正面に戻る状態
            mMotors.setSpeed(90, -90);
            if (mHeading.getHeading() > 90.0f) {
                // 正面に戻ったら再び直進状態へループする
                searchState = 0; //[cite: 38]
            }
            break;
    }
    
    // まだ完了していない
    return false; //[cite: 38]
}