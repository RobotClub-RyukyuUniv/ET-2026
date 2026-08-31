#ifndef FINDER_H
#define FINDER_H

#include <libcpp/spike/Motor.h>
#include <libcpp/spike/UltrasonicSensor.h>
#include <libcpp/spike/Display.h>

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
    Motor leftWheel;
    Motor rightWheel;
    UltrasonicSensor ultrasonicSensor;
    Display display;

    int8_t search_speed;
    
    // 裏で取得した最新の距離を保存しておく変数（volatileをつけて最適化を防ぐ）
    volatile int32_t latest_distance; 
    bool is_moving; 
};

#endif