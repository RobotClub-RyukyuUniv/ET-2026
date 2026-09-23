#include "StraightRunner.h"

StraightRunner::StraightRunner(LeftRightMotor& motors) 
    : mMotors(motors) {}

void StraightRunner::run(int speed) {
    mMotors.setSpeed(speed, speed); //[cite: 17, 37]
}

void StraightRunner::stop() {
    mMotors.stop(); //[cite: 17, 37]
}