#ifndef LINE_TRACER_H_
#define LINE_TRACER_H_

#include "../device/ColorSensorManager.h"
#include "../device/DualMotorController.h"

// テストコードの計算式に対応したパラメータをまとめる構造体
struct LineTraceParam {
    float kp;
    float ki;
    float kd;
    int target;
    float base_pwm;
    float bias;
    float k_speed;
    float k_derivative_speed;
    float min_pwm_ratio; // 例: 0.4f や 0.4925f など
};

class LineTracer {
public:
    LineTracer(ColorSensorManager& colorSensorManager, DualMotorController& dualMotorController);

    void init();
    
    // 司令塔からその都度パラメータを渡して1フレーム分を実行する
    void run(const LineTraceParam& param);

    // 積分値や前回センサー値を外部からリセットしたいとき用（暴走防止など）
    void resetIntegral() { mIntegral = 0; }

private:
    ColorSensorManager& mColorSensorManager;
    DualMotorController& mDualMotorController;

    // テストコードから引き継いだ内部保持変数
    int mIntegral;
    int mPrevSensorVal;
};

#endif  // LINE_TRACER_H_