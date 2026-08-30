#include "app.h"
#include <stdio.h>
#include <serial/newlib.h>
#include <serial/serial.h>

#include <libcpp/spike/Motor.h> 
#include <libcpp/spike/Clock.h>
#include "ColorSensor.h"

using namespace spikeapi; 

/**
 * メインタスク
 */
void main_task(intptr_t unused) {
  // USBシリアルポートを標準出力(printf)に割り当てる
  stdout = serial_open_newlib_file(SIO_USB_PORTID);

  Motor armMotor(EPort::PORT_C, Motor::EDirection::CLOCKWISE, true);
  Clock clock;

  ColorSensor colorSensor(EPort::PORT_E);

  ColorSensor::HSV color;
  ColorSensor::HSV hsv;
  int32_t reflection;

  ColorSensor::HSV color_out;
  ColorSensor::HSV hsv_out;
  int32_t ambient;

  const int8_t pwm = 25;
  const uint32_t duration = 500 * 1000;

  armMotor.setPower(-pwm);
  clock.sleep(duration);
  armMotor.hold();

  // 起動時のヘッダー表示
  printf("=== ColorSensor Logger Started ===\n");

  while(1) {
    colorSensor.getColor(color); // カラーセンサで色を測定する
    colorSensor.getHSV(hsv);     // カラーセンサで色を測定する（近似なし）
    reflection = colorSensor.getReflection(); //  反射率

    colorSensor.getColor(color_out, false); // カラーセンサで色を測定する
    colorSensor.getHSV(hsv_out, false);     // カラーセンサで色を測定する（近似なし）
    ambient = colorSensor.getAmbient(); //  環境光

    printf("\n");
    printf("ColorS[%u, %u, %u] | HSV[%u, %u, %u] | Reflection: %d\n",
            color.h, color.s, color.v,
            hsv.h, hsv.s, hsv.v,
            (int)reflection);

    printf("ColorS_out[%u, %u, %u] | HSV_out[%u, %u, %u] | Ambient: %d\n",
            color_out.h, color_out.s, color_out.v,
            hsv_out.h, hsv_out.s, hsv_out.v,
            (int)ambient);
    printf("\n");

    clock.sleep(50);
  } 
}
