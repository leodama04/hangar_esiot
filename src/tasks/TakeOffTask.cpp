#include "tasks/TakeOffTask.h"
#include "HangarDoor.h"
#include "MsgService.h"
#include "global.h"

#define D1 0.25
#define T1 10000

TakeOffTask::TakeOffTask(){}

void TakeOffTask::init(int period){
  Task::init(period);
  tStart = 0;
  timerRunning = false;
  state = WAITING_MSG;
}

void TakeOffTask::tick(){
  switch(state) {
    case WAITING_MSG: {
      MsgService.handleMessage();
      if(drone.isRequestTakeOffSent()) {
        blinkingTask.setActive(true);
        drone.consumeRequestTakeOff();
        hangarDoor.open();
        drone.setDroneTAKINGOFF();
        state = WAITING_DOOR_TO_OPEN;
      }
      break;
    }
    case WAITING_DOOR_TO_OPEN: {
      hangarDoor.update();
      if(hangarDoor.isOpen()) {
        state = WAITING_DRONE_TO_EXIT;
      }
      break;
    }
    case WAITING_DRONE_TO_EXIT: {
      float dist = DDD->getDistance();
      MsgService.sendDistance(dist);
      if(dist > D1) {
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
      if(dist <= D1) {
        timerRunning = false;
        state = WAITING_DRONE_TO_EXIT;
        break;
      }
      if(timerRunning && millis() - tStart > T1) {
        hangarDoor.close();
        timerRunning = false;
        state = WAITING_DOOR_TO_CLOSE;
      }
      break;
    }
    case WAITING_DOOR_TO_CLOSE: {
      hangarDoor.update();
      if(hangarDoor.isClosed()) {
        blinkingTask.setActive(false);
        drone.setDroneOUTSIDE();
        state = WAITING_MSG;
      }
      break;
    }
  }
}
