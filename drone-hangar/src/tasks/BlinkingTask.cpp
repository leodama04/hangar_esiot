#include "BlinkingTask.h"

BlinkingTask::BlinkingTask(){}

void BlinkingTask::init(int period, Led* led) {
    Task::init(period);
    this->led = led;
    state = TURNING_ON;
    this->setActive(false);
}

void BlinkingTask::tick() {
    switch(state) {
        case TURNING_ON: {
            led->switchOn();
            state = TURNING_OFF;
            break;
        }
        case TURNING_OFF: {
            led->switchOff();
            state = TURNING_ON;
            break;
        }
    }
}

void BlinkingTask::setActive(bool active) {
    Task::setActive(active);
    switch (active) {
        case false:
            led->switchOff();
            break;
        case true:
            led->switchOn();
            break;
    }
}

