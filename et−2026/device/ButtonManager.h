#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include "ForceSensor.h"

using namespace spikeapi;

class ButtonManager {
private:
    ForceSensor forceSensor;

public:
    // コンストラクタ（ポートDのフォースセンサを内部で初期化）
    ButtonManager();

    // コード内で使われている isPressed だけを抽出
    bool isPressed(float force = 10.0f) const;
};

#endif // BUTTON_MANAGER_H