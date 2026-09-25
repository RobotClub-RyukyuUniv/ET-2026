#include "BottlePusher.h"

BottlePusher::BottlePusher(DualMotorController& motors, ArmManager& arm, ColorSensorManager& colorSensor, TimerManager& timer)
    : mMotors(motors), mArm(arm), mColorSensor(colorSensor), mTimer(timer), pushState(0), stateTimer(0) {}

void BottlePusher::init() {
    pushState = 0;
    stateTimer = 0;
}

bool BottlePusher::run() {
    switch (pushState) {
        case 0: // 1. ボトルにぶつからないように、まず少しバックする
            mMotors.setSpeed(-30, -30); // 逆回転で後退
            stateTimer = mTimer.now();   // タイマー開始
            pushState = 1;
            break;

        case 1: // 一定時間バックしたら、止まってアームを下ろす
            // 例: 500ms (0.5秒) バックする (時間は機体に合わせて調整)
            if (mTimer.now() - stateTimer >= 500) {
                mMotors.stop();
                mArm.down(); // アームを下ろす
                stateTimer = mTimer.now(); // アームを下ろしきるためのウェイト用
                pushState = 2;
            }
            break;

        case 2: // アームがしっかり下りるまで少し待ってから、前進（押し出し）を開始
            // 例: 500ms 待機
            if (mTimer.now() - stateTimer >= 500) {
                mMotors.setSpeed(50, 50);  // 前進してボトルを押す
                stateTimer = mTimer.now(); // タイムアウト計測用の時間を記録
                pushState = 3;
            }
            break;

        case 3: // 押し出し中：「灰色を検知した」または「時間切れ（タイムアウト）」を見る
            // 灰色の検知条件（例: 反射率が一定値以下など。フィールドに合わせて調整してください）
            bool isGrayDetected = (mColorSensor.getReflection() < 30);

            // タイムアウト条件（例: 3000ms = 3秒間見つからなかったら強制終了）
            bool isTimeout = (mTimer.now() - stateTimer >= 3000);

            if (isGrayDetected || isTimeout) {
                mMotors.stop();
                mArm.up();   // アームを元に戻す（必要に応じて）
                return true; // 押し出し完了！ 呼び出し元の遷移へ
            }
            break;
    }
    return false;
}