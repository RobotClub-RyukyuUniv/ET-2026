#include "Tracer.h" // <1>
#include <stdio.h>


Tracer::Tracer():
  leftWheel(EPort::PORT_B,Motor::EDirection::COUNTERCLOCKWISE,true), // <2>
  rightWheel(EPort::PORT_A,Motor::EDirection::CLOCKWISE,true),  // <2>
  colorSensor(EPort::PORT_E) { // <2>
}

void Tracer::init() {
  printf("Tracer\n");
}

void Tracer::terminate() {
  printf("Stopped.\n");
  leftWheel.stop();  // <1>
  rightWheel.stop();
}

void Tracer::run() {
  const float Kp = 3.0;  // 比例ゲイン
  const float Ki = 0.0;  // 積分ゲイン（初期値0）
  const float Kd = 0.2;  // 微分ゲイン（初期値0）
  const int target = 15; // 目標値
  const int bias = 0;    // バイアス値

  printf("running...\n");

  // 1. 誤差の計算
  int diff = colorSensor.getReflection() - target;

  // 2. 積分項（誤差の累積）と 微分項（今回の誤差 - 前回の誤差）の計算
  integral += diff;
  int derivative = (diff - prev_diff) / 0.05;
  prev_diff = diff; // 次回のために現在の誤差を保存

  // 3. PID制御の計算
  float turn = (Kp * diff) + (Ki * integral) + (Kd * derivative) + bias;

  // 4. モーター出力の設定
  leftWheel.setPower(pwm - turn);
  rightWheel.setPower(pwm + turn);
}
