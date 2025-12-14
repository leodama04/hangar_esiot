#include "tasks/AlarmTask.h"
#include "model/global.h"

#define Temp1 24.5
#define T3 10000
#define Temp2 26
#define T4 5000


AlarmTask::AlarmTask() {}

void AlarmTask::init(int period) {
    Task::init(period);
    timerRunning = false;
    tStart = 0;
    state = NORMAL_STATE;
}

void AlarmTask::tick() {
    switch (state) {
        case NORMAL_STATE: {
            float t = tempSensor->getTemperature();
            MsgService.sendTemp(t);
            if(t > Temp1) {
                if(!timerRunning) {
                    tStart = millis();
                    timerRunning = true;
                }
                state = WAITING_PRE_ALARM;
            }
            break;
        }    
        case WAITING_PRE_ALARM: {
            float t = tempSensor->getTemperature();
            MsgService.sendTemp(t);
            if(t <= Temp1) {
                timerRunning = false;
                state = NORMAL_STATE;
                break;
            }
            if(timerRunning && millis() - tStart > T3) {
                if(drone.isDroneTAKINGOFF() || drone.isDroneLANDING()) {
                    state = WAIT_TO_COMPLETE_ACTIONS;
                }
                else {
                    landingTask.setActive(false);
                    takeOffTask.setActive(false);
                    MsgService.sendMsgPREALARM();
                    state = PRE_ALARM;
                }
                timerRunning = false;
            }
            break;
        }
        case WAIT_TO_COMPLETE_ACTIONS: {
            if(drone.isDroneINSIDE() || drone.isDroneOUTSIDE()) {
                landingTask.setActive(false);
                takeOffTask.setActive(false);
                MsgService.sendMsgPREALARM();
                state = PRE_ALARM;
            }
            break;
        }
        case PRE_ALARM: {
            float t = tempSensor->getTemperature();
            MsgService.sendTemp(t);
            if(t <= Temp1) {
                landingTask.setActive(true);
                takeOffTask.setActive(true);
                MsgService.sendMsgRESET();
                timerRunning = false;
                state = NORMAL_STATE;
                break;
            }
            if(t > Temp2) {
                if(!timerRunning) {
                    tStart = millis();
                    timerRunning = true;
                }
                state = WAITING_ALARM;
            }
            break;
        }
        case WAITING_ALARM: {
            float t = tempSensor->getTemperature();
            MsgService.sendTemp(t);
            if(t <= Temp1) {
                timerRunning = false;
                state = NORMAL_STATE;
                break;
            }
            if(t <= Temp2) {
                timerRunning = false;
                state = PRE_ALARM;
                break;
            }
            if(timerRunning && millis() - tStart > T4) {
                if(drone.isDroneOUTSIDE()) {
                    MsgService.sendMsgDRUALARM();
                }
                if(hangarDoor.isClosed()) {
                    lcd.clear();
                    lcd.print("ALARM");
                    MsgService.sendMsgALARM();
                    L3->switchOn();
                    state = ALARM;
                }
                else {
                    hangarDoor.close();
                    state = WAITING_DOOR_TO_CLOSE;
                }
                timerRunning = false;
                break;
            }
            break;
        }
        case WAITING_DOOR_TO_CLOSE: {
            hangarDoor.update();
            if(hangarDoor.isClosed()) {
                lcd.clear();
                lcd.print("ALARM");
                MsgService.sendMsgALARM();
                L3->switchOn();
                state = ALARM;
            }
            break;
        }
        case ALARM: {
            if(button->isPressed()) {
                landingTask.setActive(true);
                takeOffTask.setActive(true);
                MsgService.sendMsgRESET();
                if(drone.isDroneINSIDE()) {
                    lcd.clear();
                    lcd.print("DRONE INSIDE");  
                }
                else {
                    lcd.clear();
                    lcd.print("DRONE OUT");  
                }
                L3->switchOff();
                state = NORMAL_STATE;
            }
            break;
        }
        
    }
}