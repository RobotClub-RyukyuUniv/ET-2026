#ifndef LINE_TRACER_H_
#define LINE_TRACER_H_

// 前方宣言（ヘッダー間の結合度を下げビルド時間を短縮）
class ColorSensorManager;
class DualMotorController;

class LineTracer {
public:
    LineTracer(ColorSensorManager& colorSensorManager, DualMotorController& dualMotorController);
    virtual ~LineTracer() = default;

    void init();
    void run();

private:
    ColorSensorManager& mColorSensorManager;
    DualMotorController& mDualMotorController;

    const float mKp;
    const float mKi;
    const float mKd;
    const int mTargetReflect;
    const int mBasePwm;

    float mDiff[2];
    float mIntegral;
};

#endif  // LINE_TRACER_H_