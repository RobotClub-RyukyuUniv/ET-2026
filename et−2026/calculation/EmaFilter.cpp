#include "EmaFilter.h"

EmaFilter::EmaFilter(float alpha)
    : alpha(alpha), previousEMA(0.0f), isInitialized(false) {
}

float EmaFilter::update(float currentValue) {
    if (!isInitialized) {
        previousEMA = currentValue;
        isInitialized = true;
    } else {
        previousEMA = alpha * currentValue + (1.0f - alpha) * previousEMA;
    }
    return previousEMA;
}

float EmaFilter::get() const {
    return previousEMA;
}

void EmaFilter::reset() {
    isInitialized = false;
    previousEMA = 0.0f;
}