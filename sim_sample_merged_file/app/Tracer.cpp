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

  float turn = calc_prop_value(); // 比例制御の調整値を求める
  int pwm_l = pwm - turn;         // 基準値と調整値を使って操作量を求める
  int pwm_r = pwm + turn;
  leftWheel.setPower(pwm_l);
  rightWheel.setPower(pwm_r );
}

float Tracer::calc_prop_value() {

  // ※ パラメータ調整は行ってない(サンプルコードのまんま使っているから)
  const float Kp=0.83;  // (1)実験結果を反映する
  const int target=10;  // (2)実験結果を反映する
  const int bias=0;

  int diff = colorSensor.getReflection() - target; //  (3) 偏差を求める
  return (Kp * diff + bias);                       //  (4) 調整値を計算して返す
}
