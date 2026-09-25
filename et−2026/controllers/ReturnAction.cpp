#include "ReturnAction.h"
#include <stdio.h>

ReturnAction::ReturnAction(Odometry& odome,
                           StraightRunner& runner,
                           ColorSensorManager& colorMgr,
                           TimerManager& timerMgr)
    : odometry(odome),
      straightRunner(runner),
      colorSensorManager(colorMgr),
      timerManager(timerMgr) {}

bool ReturnAction::run() {
    printf("[ReturnAction] Starting return action sequence...\n");

    // 1. オドメトリを使って現在の位置・角度を計算・確定する
    odometry.calculatePosition(); //[cite: 12]
    float angleFromOrigin = odometry.getAngleFromOrigin(); //[cite: 12]
    float currentHeading = odometry.getRobotHeading(); //[cite: 12]

    printf("[ReturnAction] Current Odometry - Angle from origin: %.2f deg, Robot Heading: %.2f deg\n", 
           angleFromOrigin, currentHeading);

    // 2. StraightRunner を使って指定方向へ移動しながら黒線を探す
    printf("[ReturnAction] Moving forward using StraightRunner and searching for black line...\n");
    straightRunner.run(SPEED_MOVE); //

    while (true) {
        // オドメトリの移動ステップを記録
        odometry.recordStep(); //

        // カラーセンサーの反射率を取得して黒線判定
        int32_t reflection = colorSensorManager.getReflection(); //[cite: 3, 7]

        // 黒線検知のしきい値（環境に合わせて調整）
        if (reflection <= 10 && reflection >= 0) {
            printf("[ReturnAction] Black line detected! Reflection: %ld\n", reflection);
            break;
        }

        timerManager.sleep(10 * 1000); // 10ms待機[cite: 6]
    }

    printf("[ReturnAction] Return action completed. Black line found.\n");

    return true;
}