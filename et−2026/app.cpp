#include <cstdint>
#include "app.h"
#include <stdio.h>

#include "ButtonManager.h"
#include "CourseRunManager.h"
#include "TimerManager.h"
#include "ArmManager.h"
#include "LineTracer.h"
#include "ColorSensorManager.h"
#include "DualMotorController.h"
#include "HeadingSensorManager.h"
#include "UltrasonicDistanceSensorManager.h"

// --- 新規コンポーネントのインクルード ---
#include "Rotator.h"
#include "StraightRunner.h"
#include "BottleExtractor.h"
#include "ObjectCenterAligner.h"
#include "Odometry.h"
#include "BottleSearcher.h"
#include "BottlePusher.h"
#include "ReturnAction.h"

// 1. デバイス・マネージャーのインスタンス化
TimerManager timerManager;
ArmManager armManager;
ColorSensorManager colorSensorManager;
DualMotorController dualMotorController;
UltrasonicDistanceSensorManager ultrasonicDistanceSensorManager;

spikeapi::IMU imu;
HeadingSensorManager headingSensorManager(imu);
LineTracer lineTracer(colorSensorManager, dualMotorController);

// 2. ミドルウェア（ET相撲用の単機能クラス）のインスタンス化
Rotator rotator(headingSensorManager, dualMotorController);
StraightRunner straightRunner(dualMotorController);
BottleExtractor bottleExtractor(colorSensorManager, ultrasonicDistanceSensorManager, 0.0f, 0);
ObjectCenterAligner centerAligner(colorSensorManager, headingSensorManager, dualMotorController, timerManager);
Odometry odometry(dualMotorController, headingSensorManager);

// 3. アクション（複数のミドルウェアを束ねるクラス）のインスタンス化
BottleSearcher bottleSearcher(bottleExtractor, rotator, straightRunner, centerAligner, headingSensorManager, dualMotorController, timerManager);
BottlePusher bottlePusher(dualMotorController, armManager, colorSensorManager, timerManager);
ReturnAction returnAction(odometry, straightRunner, colorSensorManager, timerManager);

// 4. 司令塔（CourseRunManager）の初期化
CourseRunManager runManager(lineTracer, colorSensorManager, timerManager, dualMotorController,
                            armManager, headingSensorManager,
                            odometry, bottleSearcher, bottlePusher, returnAction);

// 周期タスク
void tracer_task(intptr_t exinf)
{
  runManager.update();
  ext_tsk();
}

void main_task(intptr_t unused)
{
  ButtonManager buttonManager;
  const uint64_t duration = 100 * 1000;

  runManager.init();

  printf("Lowering arm...\n");
  armManager.down();
  printf("Arm reached the limit and is now held in position.\n");

  bool is_running = false;
  bool prev_touched = false;
  uint64_t press_start_time = timerManager.now();
  bool is_measuring = false;

  while (true)
  {
    bool current_touched = buttonManager.isPressed(10.0);

    if (current_touched && !prev_touched)
    {
      press_start_time = timerManager.now();
      is_measuring = true;
    }

    if (current_touched && is_measuring)
    {
      if (timerManager.now() - press_start_time >= 2000000ULL)
      {
        break;
      }
    }

    if (!current_touched && prev_touched)
    {
      if (is_measuring)
      {
        is_running = !is_running;

        if (is_running)
        {
          printf("Start course run manager!\n");
          sta_cyc(TRACER_CYC);
        }
        else
        {
          printf("Stop course run manager.\n");
          stp_cyc(TRACER_CYC);
        }
        is_measuring = false;
      }
    }

    prev_touched = current_touched;
    timerManager.sleep(duration);
  }

  if (is_running)
  {
    stp_cyc(TRACER_CYC);
  }
  printf("Program terminated.\n");
  ext_tsk();
}

#ifdef __cplusplus
extern "C"
{
#endif
  void *__dso_handle = nullptr;
  void _fini(void) {}
#ifdef __cplusplus
}
#endif