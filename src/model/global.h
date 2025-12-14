#ifndef __GLOBAL__
#define __GLOBAL__

#include "Drone.h"
#include "HangarDoor.h"
#include "kernel/MsgService.h"
#include "devices/ProximitySensor.h"
#include "devices/Sonar.h"
#include "devices/PresenceSensor.h"
#include "devices/Pir.h"
#include "devices/Led.h"
#include "devices/TempSensorTMP.h"
#include "devices/TempSensor.h"
#include "devices/Button.h"
#include "devices/ButtonImpl.h"
#include "tasks/BlinkingTask.h"
#include "tasks/TakeOffTask.h"
#include "tasks/LandingTask.h"
#include "LiquidCrystal_I2C.h"


#define MOT_PIN 9
#define ECHO_PIN 7
#define TRIG_PIN 8 
#define PIR_PIN 3
#define L2_PIN 6
#define L1_PIN 12
#define L3_PIN 11
#define TMP_PIN A0
#define BUT_PIN 2

extern Drone drone;
extern HangarDoor hangarDoor;
extern ProximitySensor* DDD;
extern PresenceSensor* DPD;
extern Led* L2;
extern Led* L1;
extern Led* L3;
extern BlinkingTask blinkingTask;
extern LandingTask landingTask;
extern TakeOffTask takeOffTask;
extern TempSensor* tempSensor;
extern Button* button;
extern LiquidCrystal_I2C lcd;   

void globalSetup();

#endif