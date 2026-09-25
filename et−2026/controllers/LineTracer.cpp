#include "LineTracer.h"
#include <cmath>
#include <algorithm>

LineTracer::LineTracer(ColorSensorManager& colorSensorManager, DualMotorController& dualMotorController)
    : mColorSensorManager(colorSensorManager),
      mDualMotorController(dualMotorController),
      mIntegral(0),
      mPrevSensorVal(0) {
}

void LineTracer::init() {
    mIntegral = 0;
    mPrevSensorVal = 0;
    mDualMotorController.stop();
}

void LineTracer::run(const LineTraceParam& param) {
    // センサ値の取得
    int sensor_val = mColorSensorManager.getReflection();

    // 偏差の計算[cite: 24]
    int diff = sensor_val - param.target;
    mIntegral += diff;
    
    // 微分の計算[cite: 24]
    int derivative = sensor_val - mPrevSensorVal;
    mPrevSensorVal = sensor_val;

    // 制御入力 turn の算出（テストコードの式を再現）[cite: 24]
    float turn = (param.kp * diff) - (param.ki * mIntegral) + (param.kd * derivative) + param.bias;

    // 速度と減速の計算[cite: 24]
    float abs_turn = std::abs(turn);
    float abs_derivative = std::abs(derivative);
    
    float current_pwm = param.base_pwm - (abs_turn * param.k_speed) - (abs_derivative * param.k_derivative_speed);
    
    // 最小PWMの制限[cite: 24]
    float min_pwm = param.base_pwm * param.min_pwm_ratio;
    current_pwm = std::max(current_pwm, min_pwm);

    // 左右モータの出力配分[cite: 24]
    float raw_left  = current_pwm - turn;
    float raw_right = current_pwm + turn;

    // モータ出力のクリッピング処理（上限100）[cite: 24]
    const float MOTOR_MAX = 100.0f;
    float max_val = std::max(std::abs(raw_left), std::abs(raw_right));
    if (max_val > MOTOR_MAX) {
        raw_left  = (raw_left / max_val) * MOTOR_MAX;
        raw_right = (raw_right / max_val) * MOTOR_MAX;
    }

    // デバイスコントローラーへ指令を送信
    mDualMotorController.setPwm(static_cast<int>(raw_left), static_cast<int>(raw_right));
}