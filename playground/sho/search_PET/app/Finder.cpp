#include "Finder.h"
#include <stdio.h>

Finder::Finder() :
    leftWheel(EPort::PORT_B, Motor::EDirection::COUNTERCLOCKWISE, true),
    rightWheel(EPort::PORT_A, Motor::EDirection::CLOCKWISE, true),
    ultrasonicSensor(EPort::PORT_F),
    search_speed(100),
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
    // 初めて探索モードに入ったときの初期化
    if (!is_moving) {
        is_moving = true;
        current_state = SearchState::FORWARD; // 最初は「前進」からスタート
        state_timer = 0;
    }

    // タイマーを進める（メインタスクから10msごとに呼ばれるため）
    state_timer++;

    // 現在の状態ごとに処理を分ける
    switch (current_state) {
    case SearchState::FORWARD:
        // 【前進の動き】
        leftWheel.setSpeed(search_speed);
        rightWheel.setSpeed(search_speed * 2);

        // 1秒（10ms × 100回）経ったら「旋回」の状態に切り替える
        if (state_timer >= 300) {
            current_state = SearchState::TURN_RIGHT;
            state_timer = 0; // タイマーをリセット
            printf("Switch to: TURN_RIGHT\n");
        }
        break;

    case SearchState::TURN_RIGHT:
        // 【旋回の動き】
        leftWheel.setSpeed(-search_speed * 0.5);
        rightWheel.setSpeed(search_speed * 0.5);

        // 1秒経ったら再び「前進」の状態に戻す（これでループする）
        if (state_timer >= 250) {
            current_state = SearchState::TURN_LEFT;
            state_timer = 0; // タイマーをリセット
            printf("Switch to: TURN_LEFT\n");
        }
        break;

    case SearchState::TURN_LEFT:
        // 【旋回の動き】
        leftWheel.setSpeed(search_speed * 0.5);
        rightWheel.setSpeed(-search_speed * 0.5);

        // 1秒経ったら再び「前進」の状態に戻す（これでループする）
        if (state_timer >= 450) {
            current_state = SearchState::FORWARD;
            state_timer = 0; // タイマーをリセット
            printf("Switch to: FORWARD\n");
        }
        break;
    }
}