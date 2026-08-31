#ifndef FINDER_H
#define FINDER_H

#include <libcpp/spike/Motor.h>
#include <libcpp/spike/UltrasonicSensor.h>
#include <libcpp/spike/Display.h>
#include <libcpp/spike/Clock.h>

using namespace spikeapi;

class Finder {
public:
    Finder();
    void init();
    void terminate();
    void run(); // 50msごとに呼ばれる（超音波取得専用）

    // メインタスクから呼び出すための関数群
    int32_t getLatestDistance(); // 最新の距離を取得する
    void stopMoving();           // 止まる
    void startSearching();       // 旋回する

private:
    // 状態に名前をつける
    enum class SearchState {
        FORWARD, // 前進する状態
        TURN_RIGHT,  // 右に旋回する状態
        TURN_LEFT   // 左に旋回する状態
    };

    Motor leftWheel;
    Motor rightWheel;
    UltrasonicSensor ultrasonicSensor;
    Display display;

    int8_t search_speed;

    SearchState current_state; // 現在の状態を保存する変数
    int state_timer;           // その状態が始まってからの経過時間（カウンタ）
    // 裏で取得した最新の距離を保存しておく変数（volatileをつけて最適化を防ぐ）
    volatile int32_t latest_distance; 
    bool is_moving; 
};

#endif