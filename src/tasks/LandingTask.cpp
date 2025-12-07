#include "tasks/LandingTask.h"
#include "HangarDoor.h"
#include "MsgService.h"
#include "global.h"

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
                drone.setDroneINSIDE();
                hangarDoor.close();
                timerRunning = false;
                state = WAITING_DOOR_TO_CLOSE;
            }
            break;
        }
        case WAITING_DOOR_TO_CLOSE: {
            hangarDoor.update();
            if(hangarDoor.isClosed()) {
                state = WAITING_MSG;
            }
            break;
        }
    } 
}
