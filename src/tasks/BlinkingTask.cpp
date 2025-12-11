#include "BlinkingTask.h"

BlinkingTask::BlinkingTask(){}

void BlinkingTask::init(int period, LedPwm* led) {
    Task::init(period);
    this->led = led;
    state = TURNING_ON;
    this->setActive(false);
}

void BlinkingTask::tick() {
    switch(state) {
        case TURNING_ON: {
            led->setIntensity(led->getIntensity() + 5); 
            if(led->getIntensity() == 255) { 
                state = TURNING_OFF; 
            }
            break;
        }
        case TURNING_OFF: {
            led->setIntensity(led->getIntensity() - 5); 
            if(led->getIntensity() == 0) { 
                state = TURNING_OFF; 
            }
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
            led->setIntensity(0);
            led->switchOn();
            break;
    }
}

