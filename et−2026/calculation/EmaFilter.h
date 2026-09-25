#ifndef EMA_FILTER_H
#define EMA_FILTER_H

class EmaFilter {
private:
    float alpha;         // 追従係数
    float previousEMA;   // 前回のEMA値
    bool isInitialized;  // 初回値フラグ

public:
    // コンストラクタ
    EmaFilter(float alpha);

    // 新しい値を受け取り、EMAを更新して計算結果を返す
    float update(float currentValue);

    // 現在保持しているEMAの値を取得する（計算前の値を確認したい時に使用）
    float get() const;

    // リセット用
    void reset();
};

#endif