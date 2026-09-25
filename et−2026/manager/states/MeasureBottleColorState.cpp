#include "MeasureBottleColorState.h"
#include <stdio.h>

RunState MeasureBottleColorState::update(CourseContext& ctx) {
    ctx.dualMotor.stop();
    
    spikeapi::ColorSensor::HSV hsv;
    ctx.colorSensor.getSurfaceHSV(hsv);
    printf("[CourseRunManager] Bottle Color measured! H: %d S: %d, V: %d\n", hsv.h, hsv.s, hsv.v);

    // 取得したH値（色相）からターゲットのマーカー回数を決定（下から数えて: 1番目=黄, 2番目=青, 3番目=赤）
    if (hsv.h >= 200 && hsv.h <= 240) {
        ctx.targetMarkerCount = 2;      // 青なら2番目
        printf("Target: BLUE (2nd marker)\n");
    } else if (hsv.h >= 330 || hsv.h <= 30) {
        ctx.targetMarkerCount = 3;      // 赤なら3番目
        printf("Target: RED (3rd marker)\n");
    } else {
        ctx.targetMarkerCount = 1;      // 黄なら1番目
        printf("Target: YELLOW (1st marker)\n");
    }

    // マーカーカウント用変数のリセットと、足元の青色無視設定
    ctx.blueMarkerCount = 0;
    ctx.isIgnoringBlue = true; 
    ctx.ignoreStartTime = ctx.timer.now();

    return RunState::PREPARE_RESUME; // 次の状態（復帰準備）へ即座に遷移
}