#ifndef BOTTLE_SEARCHER_H_
#define BOTTLE_SEARCHER_H_

// 必要なセンサー類のインクルード省略...

class BottleSearcher {
public:
    BottleSearcher(/* センサー類の参照を受け取る */);

    // UML定義メソッド
    // 探索が完了（ボトルを捕捉）したら true を返す
    bool run();

private:
    // UML定義属性
    int searchState;
};

#endif // BOTTLE_SEARCHER_H_