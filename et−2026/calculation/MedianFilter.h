#ifndef MEDIAN_FILTER_H
#define MEDIAN_FILTER_H

#include <stddef.h>
#include <stdint.h>

class MedianFilter {
private:
    float* buffer;       // 履歴を保持するリングバッファ
    size_t bufferSize;   // バッファのサイズ（例: 5）
    size_t currentIndex; // 現在の書き込み位置

public:
    // コンストラクタ（サイズと初期値を受け取る。app_2.cppの初期値70に対応）
    MedianFilter(size_t size, float initialValue = 70.0f);
    
    // デストラクタ
    ~MedianFilter();

    // 新しい値を受け取り、中央値を計算して返す
    float update(float currentValue);

    // リセット用
    void reset(float initialValue = 70.0f);
};

#endif