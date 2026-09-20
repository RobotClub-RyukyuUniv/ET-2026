#include "LineTracer.h"
#include "ColorSensorManager.h"
#include "DualMotorController.h"

LineTracer::LineTracer(ColorSensorManager& colorSensorManager, DualMotorController& dualMotorController)
    : mColorSensorManager(colorSensorManager),
      mDualMotorController(dualMotorController),
      mKp(0.8f),
      mKi(0.0f),
      mKd(0.1f),
      mTargetReflect(20),
      mBasePwm(30),
      mDiff{0.0f, 0.0f},
      mIntegral(0.0f) {
}

void LineTracer::init() {
    mDiff[0] = 0.0f;
    mDiff[1] = 0.0f;
    mIntegral = 0.0f;
    mDualMotorController.stop();
}

void LineTracer::run() {
    // カラーセンサーから現在の反射光強度を取得
    int currentReflect = mColorSensorManager.getBrightness();

    // 偏差の更新と積分値の計算
    mDiff[0] = mDiff[1];
    mDiff[1] = static_cast<float>(currentReflect - mTargetReflect);
    mIntegral += (mDiff[0] + mDiff[1]) / 2.0f;

    // P, I, D 制御量の計算
    float p = mKp * mDiff[1];
    float i = mKi * mIntegral;
    float d = mKd * (mDiff[1] - mDiff[0]);

    int turn = static_cast<int>(p + i + d);

    // 左右モーターのPWM出力を計算して設定
    int leftPwm = mBasePwm + turn;
    int rightPwm = mBasePwm - turn;

    mDualMotorController.setPWM(leftPwm, rightPwm);
}