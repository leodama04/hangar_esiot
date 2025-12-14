#include "tasks/LandingTask.h"
#include "model/HangarDoor.h"
#include "kernel/MsgService.h"
#include "model/global.h"

#define D2 0.10
#define T2 5000

LandingTask::LandingTask(){}

void LandingTask::init(int period) {
    Task::init(period);
    tStart = 0;
    timerRunning = false;
    state = WAITING_MSG;
}

void LandingTask::tick() {
    switch (state) {
        case WAITING_MSG: {
            MsgService.handleMessage();
            if(drone.isRequestLandingSent()) {
                drone.consumeRequestLanding();
                state = DETECTING_DRONE;
            }
            break;
        }
        case DETECTING_DRONE: {
            if(DPD->isDetected()) {
                lcd.clear();
                lcd.print("LANDING");
                blinkingTask.setActive(true);
                hangarDoor.open();
                drone.setDroneLANDING();
                state = WAITING_DOOR_TO_OPEN;
            }
            break;
        }
        case WAITING_DOOR_TO_OPEN: {
            hangarDoor.update();
            if(hangarDoor.isOpen()) {
                state = WAITING_DRONE_TO_ENTER;
            }
            break;
        }
        case WAITING_DRONE_TO_ENTER: {
            float dist = DDD->getDistance();
            MsgService.sendDistance(dist);
            if(dist < D2) {
                if(!timerRunning) {
                    tStart = millis();
                    timerRunning = true;
                }
                state = WAITING_TO_CLOSE_DOOR;
            }
            break;
        }
        case WAITING_TO_CLOSE_DOOR: {
            float dist = DDD->getDistance();
            MsgService.sendDistance(dist);
            if(dist >= D2) {
                timerRunning = false;
                state = WAITING_DRONE_TO_ENTER;
                break;
            }
            if(timerRunning && millis() - tStart > T2) {
                hangarDoor.close();
                timerRunning = false;
                state = WAITING_DOOR_TO_CLOSE;
            }
            break;
        }
        case WAITING_DOOR_TO_CLOSE: {
            hangarDoor.update();
            if(hangarDoor.isClosed()) {
                lcd.clear();
                lcd.print("DRONE INSIDE");
                L1->switchOn();
                blinkingTask.setActive(false);
                drone.setDroneINSIDE();
                state = WAITING_MSG;
            }
            break;
        }
    } 
}
