#include "Finder.h"
#include <stdio.h>

Finder::Finder() :
    leftWheel(EPort::PORT_B, Motor::EDirection::COUNTERCLOCKWISE, true),
    rightWheel(EPort::PORT_A, Motor::EDirection::CLOCKWISE, true),
    ultrasonicSensor(EPort::PORT_F),
    search_speed(10),
    latest_distance(-1),
    is_moving(false)
{
}

void Finder::init() {
    printf("Finder initialized.\n");
}

void Finder::terminate() {
    stopMoving();
    printf("Finder terminated.\n");
}

// ★ 50msごとに呼ばれる職人タスク（これしかやらない！）
void Finder::run() {
    // 距離を測って、変数に最新値として保存するだけ
    latest_distance = ultrasonicSensor.getDistance();
}

// ★ メインタスクが最新の距離を知りたい時に呼ぶ関数
int32_t Finder::getLatestDistance() {
    return latest_distance;
}

// ★ メインタスクが「止まれ！」と指示した時に呼ばれる関数
void Finder::stopMoving() {
    if (is_moving) {
        leftWheel.stop();
        rightWheel.stop();
        is_moving = false;
        printf("Catched! (Stopped)\n");
    }
}

// ★ メインタスクが「探せ！」と指示した時に呼ばれる関数
void Finder::startSearching() {
    if (!is_moving) {
        leftWheel.setSpeed(-search_speed);
        rightWheel.setSpeed(search_speed);
        is_moving = true;
        printf("Searching...\n");
    }
}