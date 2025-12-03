#include "tasks/TakeOffTask.h"
#include "HangarDoor.h"
#include "MsgService.h"
#include "global.h"


TakeOffTask::TakeOffTask(){
  state = WAITING_MSG;
}

void TakeOffTask::init(int period){
  Task::init(period);
}

void TakeOffTask::tick(){
  switch(state) {
    case WAITING_MSG:
      MsgService.handleMessage();
      if(drone.isRequestTakeOffSent()) {
        drone.consumeRequestTakeOff();
        hangarDoor.open();
        drone.setDroneTAKINGOFF();
        state = WAITING_DOOR_TO_OPEN;
      }
      break;
    case WAITING_DOOR_TO_OPEN:
      hangarDoor.update();
      if(hangarDoor.isOpen()) {
        drone.setDroneOUTSIDE();
        state = WAITING_MSG;
      }
      break;
  }
}
