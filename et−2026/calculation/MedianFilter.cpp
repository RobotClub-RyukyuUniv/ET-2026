#include "MedianFilter.h"

MedianFilter::MedianFilter(size_t size, float initialValue)
    : bufferSize(size), currentIndex(0) {
    buffer = new float[bufferSize];
    for (size_t i = 0; i < bufferSize; ++i) {
        buffer[i] = initialValue;
    }
}

MedianFilter::~MedianFilter() {
    delete[] buffer;
}

float MedianFilter::update(float currentValue) {
    // 1. 新しい値をリングバッファに格納
    buffer[currentIndex] = currentValue;
    currentIndex = (currentIndex + 1) % bufferSize;

    // 2. ソート用のテンポラリ配列を作成してコピー（最大10要素まで安全に対応）
    float temp_sorted[10];
    size_t size = (bufferSize < 10) ? bufferSize : 10;
    for (size_t i = 0; i < size; ++i) {
        temp_sorted[i] = buffer[i];
    }

    // 3. バブルソート
    for (size_t i = 0; i < size - 1; ++i) {
        for (size_t j = 0; j < size - 1 - i; ++j) {
            if (temp_sorted[j] > temp_sorted[j + 1]) {
                float tmp = temp_sorted[j];
                temp_sorted[j] = temp_sorted[j + 1];
                temp_sorted[j + 1] = tmp;
            }
        }
    }

    // 4. 中央値を返す（サイズが5ならインデックス2）
    return temp_sorted[size / 2];
}

void MedianFilter::reset(float initialValue) {
    currentIndex = 0;
    for (size_t i = 0; i < bufferSize; ++i) {
        buffer[i] = initialValue;
    }
}