#include "ButtonManager.h"

using namespace spikeapi;

// ポートDで ForceSensor を初期化
ButtonManager::ButtonManager()
    : forceSensor(EPort::PORT_D) {
}

bool ButtonManager::isPressed(float force) const {
    return forceSensor.isPressed(force);
}