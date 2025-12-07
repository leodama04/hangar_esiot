#ifndef __GLOBAL__
#define __GLOBAL__

#include "Drone.h"
#include "HangarDoor.h"
#include "MsgService.h"
#include "devices/ProximitySensor.h"
#include "devices/Sonar.h"
#include "devices/PresenceSensor.h"
#include "devices/Pir.h"

#define MOT_PIN 9
#define ECHO_PIN 7
#define TRIG_PIN 8 
#define PIR_PIN 3

extern Drone drone;
extern HangarDoor hangarDoor;
extern ProximitySensor* DDD;
extern PresenceSensor* DPD;

void globalSetup();

#endif